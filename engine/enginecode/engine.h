/** \file engine.h
* A header file which includes relevant headers for development
*/

#pragma once

#include "core/application.h"

#include "systems/log.h"
#include "systems/timer.h"
#include "systems/windows.h"
#include "systems/layerStack.h"
#include "systems/resourceManager.h"

#include "windows/window.h"

#include "events/event.h"
#include "events/windowsEvents.h"
#include "events/keyEvents.h"
#include "events/codes.h"
#include "events/inputPolling.h"

#include "rendering/renderer.h"
#include "rendering/renderCommands.h"
#include "rendering/textLabel.h"
#include "rendering/batchedTextLabel.h"
#include "rendering/cameras/fixedOrthoCameraController2D.h"
#include "rendering/cameras/freeOrthoCameraController2D.h"
#include "rendering/cameras/FPSCameraControllerQuat.h"
#include "rendering/cameras/FPSCameraControllerEuler.h"

#include "components/gameObject.h"
#include "components/renderingComponents.h"
#include "components/physicsComponents.h"
#include "components/controllerComponents.h"

#include "core/JSONLoaderLog.h"
#include "core/JSONLoader.h"
#include "rendering/JSONLayer.h"
#include "rendering/IMGuiLayer.h"