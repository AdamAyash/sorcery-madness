#pragma once

#include <Common/Common.h>

namespace engine
{
	class GameTime final : common::NonCopyableObject
	{
	public:
		GameTime();
		~GameTime();

	public:
		/// <summary>
		/// Calculates diffrent time variables.
		/// </summary>
		/// <returns>double</returns>
		void Calculate();

		/// <summary>
		/// Returns delta time in seconds.
		/// </summary>
		/// <returns>double</returns>
		double GetDeltaTtime() const;

		/// <summary>
		/// Returns the total game time in seconds since glfw was initialized.
		/// </summary>
		/// <returns>double</returns>
		double GetTotalGameTime() const;

	private:
		double m_dCurrentTime = 0;
		double m_dLastFrame = 0;
		double m_dDeltaTime = 0;
	};
}