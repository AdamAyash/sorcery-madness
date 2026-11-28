#pragma once

namespace networking
{
	// Maximum packet size
	constexpr int n_MAX_PACKET_SIZE = 1024;
	constexpr int n_MAX_PACKET_RETRY_TIMEOUT_IN_SECONDS = 3;
	constexpr int n_MAX_PACKET_RETRY_COUNT = 3;
	constexpr int n_MAX_PACKET_RETRY_TIMEOUT_IN_SECONDS_FOR_UNLIMITED_POLICY = 30;

	constexpr int n_MAX_PLAYER_SESSIONS_SUPPORTED = 5;
}