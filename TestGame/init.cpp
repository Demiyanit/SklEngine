#include <entry.hpp>
#include "TestGame.hpp"

IApplication* GetConfiguredApplication() {
	return new TestGame();
}