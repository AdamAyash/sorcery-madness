# Надеждна доставка: изричен ACK с ключ (адрес, ID)

**Дата:** 2026-11-25
**Покрива находки от [CODE_REVIEW.md](CODE_REVIEW.md):** H1, H2, H3, C6

Всеки надежден пакет се потвърждава изрично, а ключът е **(адрес, ID)**. Така работи в двете посоки и сървърът може да праща собствени надеждни събития.

---

## Принципът

1. Всеки пакет, който трябва да бъде потвърден, носи флаг **„искам ACK“**.
2. Получателят **винаги** връща `Ack { nnAckId = request id }` на подателя, **дори при дубликат**, защото предишният ACK може да се е изгубил.
3. Получателят помни кои (адрес, ID) вече е обработил и **не обработва повторно** дубликатите.
4. Изпращачът пази чакащите пакети по ключ **(адрес на получателя, ID)** и ги маха само при ACK **от същия адрес**.
5. ACK пакетът е ненадежден: за него не се чака потвърждение, иначе потвържденията ще продължат безкрайно.

```
Клиент (127.0.0.1:50000)                   Сървър
Connect id=1 ack?=да ───────────────────►  няма го в историята → обработва → сесия 1
  pending: {(сървър,1)}                    ◄── Ack{1}                       ✗ губи се
                                           ConnectionEstablished id=7 ack?=да
                                             pending: {(клиент,7)}
  ◄─────────────────────────────────────── ConnectionEstablished id=7
  Ack{7} ───────────────────────────────►  маха (клиент,7) ✅
  Connected, сесия 1
(3 s) Connect id=1 (повторно, ACK-ът се изгуби)
  ───────────────────────────────────────► (клиент,1) е в историята → само Ack{1},
                                            без обработка, без „already connected“
  ◄── Ack{1}: маха (сървър,1) ✅
```

Повторният `Connect` вече не стига до `ProcessPacket`, защото го спира историята. Така C6 се решава още на ниво транспорт.

---

## Промени по файлове

### 1. [PacketHeader.h](networking/source/Networking/Packets/PacketHeader.h) и нов тип пакет

```cpp
__int64 nnRequestId = 0;
bool bRequiresAck = false;            // слага се от SendPacket при Reliable
PacketRetryPolicy eRetryPolicy = PacketRetryPolicy::Limited;
PacketTypes ePacketType = PacketTypes::Undefined;
```

```cpp
// PacketTypes: добави Ack
struct AckPacket : public Packet
{
    AckPacket() { ePacketType = PacketTypes::Ack; }
    __int64 nnAckId = 0;              // ID-то, което потвърждаваме
};
```

`ReceivedPacket` също трябва да получи `bRequiresAck`. Вдигни `ProtocolVersion`, защото хедърът се променя.

### 2. [SocketAddress](networking/source/Networking/SocketAddress.h): ключ за адреса

```cpp
// IPv4 (32 бита) + порт (16 бита) → едно число, удобно за ключ
uint64_t ToKey() const
{
    const auto* p = reinterpret_cast<const sockaddr_in*>(&m_oRawSocketAddress);
    return (static_cast<uint64_t>(p->sin_addr.s_addr) << 16) | p->sin_port;
}
```

### 3. [PendingPacket.h](networking/source/Networking/Packets/PendingPacket.h): адрес на получателя (H3)

```cpp
SocketAddress oDestinationAddress;   // вече е добавено
```

### 4. [PendingPacketsQueueManager](networking/source/Networking/PendingPacketsQueueManager/PendingPacketsQueueManager.h): ключ (адрес, ID)

```cpp
using PendingPacketKey = std::pair<uint64_t, __int64>;     // (адрес, request id)

void Submit(IN PendingPacket& oPendingPacket);             // ключ = {oDestinationAddress.ToKey(), id}
void Remove(IN const SocketAddress& oFrom, IN const __int64 nnAckId);

std::map<PendingPacketKey, PendingPacket> m_oPendingPacketsQueue;   // std::map → не трябва hash
```

`FindPendingPacket` вече не е нужен. Препоръчвам и мениджърът да стане **член на `NetworkEndpoint`**, а не singleton. С ключа по адрес работи и като singleton, но като член зависимостта се вижда.

### 5. [NetworkEndpoint.h](networking/source/Networking/NetworkEndpoint.h)

**`SendPacket` при Reliable:**

```cpp
pBasePacket->bRequiresAck = true;
oPendingPacket.oDestinationAddress = pSocketAddress ? *pSocketAddress : m_oSocket->GetAddress(); // клиентът → сървъра
```

Ако `BaseSocket` няма getter за `m_oSocketAddress`, трябва да се добави.

**`ReceivePackets`, след `ValidatePacketHeader`:**

```cpp
// а) ACK: маха чакащия пакет и не влиза в опашката за обработка
if (pPacketHeader->ePacketType == PacketTypes::Ack)
{
    const auto* pAck = reinterpret_cast<const AckPacket*>(messageBuffer);
    m_oPendingPacketsQueueManager.Remove(oClientAddress, pAck->nnAckId);
    continue;
}

// б) Иска ли ACK: потвърждаваме ВИНАГИ, дори при дубликат
if (pPacketHeader->bRequiresAck)
{
    AckPacket oAck;
    oAck.nnAckId = pPacketHeader->nnRequestId;
    SendPacket(oAck, &oClientAddress);                    // Unreliable
}

// в) Дубликат: потвърден е, но не се обработва повторно
if (IsAlreadyReceived(oClientAddress, pPacketHeader->nnRequestId))
    continue;
MarkAsReceived(oClientAddress, pPacketHeader->nnRequestId);
```

**История на получените пакети в базовия клас.** Тя заменя `IsPacketAlreadyQueued` в Client и Server, който хваща дубликати само в рамките на един тик:

```cpp
static constexpr size_t n_RECEIVED_HISTORY_SIZE = 64;
std::unordered_map<uint64_t, std::deque<__int64>> m_oReceivedHistory;   // адрес → последните 64 ID

bool IsAlreadyReceived(const SocketAddress& oFrom, __int64 nnId) const
{
    const auto it = m_oReceivedHistory.find(oFrom.ToKey());
    return it != m_oReceivedHistory.end() && std::ranges::find(it->second, nnId) != it->second.end();
}

void MarkAsReceived(const SocketAddress& oFrom, __int64 nnId)
{
    auto& oHistory = m_oReceivedHistory[oFrom.ToKey()];
    oHistory.push_back(nnId);
    if (oHistory.size() > n_RECEIVED_HISTORY_SIZE)
        oHistory.pop_front();
}
```

**`ProcessPendingPackets`:** повторното изпращане вече отива на правилния адрес.

```cpp
SendPacketInternal(pPacketData, nPacketSizeInBytes, &oPendingPacket.oDestinationAddress);
```

### 6. [Server.cpp](server/source/Server.cpp), `Run`

Добави `ProcessPendingPackets();` след `ReceivePackets();` (H2).

### 7. [ServerPacketProcessor.cpp](server/source/ServerPacketProcessor/ServerPacketProcessor.cpp)

`ConnectionEstablished` вече **не** копира `nnRequestId` от `Connect`. Получава собствен номер и се праща като `Reliable`. Връзката „кой на кого отговаря“ вече се носи от ACK-а.

---

## На какво да внимаваш

- **Рестарт на клиента от същия порт.** Новият клиент пак започва от `Connect id=1`, а сървърът още помни (адрес, 1) като обработен, затова само потвърждава и не създава сесия. Решението е клиентът да започва `m_nnNextRequestId` от **случайно** число, например `std::random_device` в конструктора. Друг вариант е сървърът да чисти историята на адреса при нов `Connect`.
- **Изчерпани опити.** Когато пакет към даден адрес бъде изпуснат след `n_MAX_PACKET_RETRY_COUNT` опита, на практика връзката е прекъсната. Засега логът е достатъчен. По-нататък това е сигнал за disconnect на сесията.
- **`Unlimited` при `Connect`.** Таймаутът расте без горна граница (M9), затова сложи таван, например 10 s.
- **`Server::Update`** праща един и същ `UpdateWorldState` обект до всички клиенти със същото ID. С ключ (адрес, ID) това вече не е проблем, а и пакетът е ненадежден.

---

## Какво решава

| Находка | Резултат |
|---|---|
| H1: смесени ID-та | ✅ ACK е изричен, а ключът включва адреса |
| H2: сървърът не повтаря | ✅ `ProcessPendingPackets` в `Run` |
| H3: грешен адрес при повторение | ✅ `oDestinationAddress` в `PendingPacket` |
| C6: повторен `Connect` | ✅ спира се от историята, преди да стигне до логиката |
| `IsPacketAlreadyQueued` в Client и Server | заменя се с обща история в базовия клас |
