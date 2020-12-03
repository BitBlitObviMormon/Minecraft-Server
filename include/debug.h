#pragma once

// Any function that has the DEPRECATED tag will be marked as deprecated and throw compiler errors.

// If we're debugging then use the visual leak detector - DISABLED, not up-to-date.
#ifdef _DEBUG
// #include "vld.h"
#endif
