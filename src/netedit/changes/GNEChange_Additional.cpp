/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    GNEChange_Additional.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Nov 2015
/// @version $Id$
///
// A network change in which a busStop is created or deleted
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <utils/common/MsgHandler.h>
#include <netedit/GNENet.h>
#include <netedit/netelements/GNELane.h>
#include <netedit/netelements/GNEEdge.h>
#include <netedit/additionals/GNEAdditional.h>
#include <netedit/GNEViewNet.h>
#include <netedit/additionals/GNEDetectorE3.h>
#include <netedit/additionals/GNEDetectorEntry.h>
#include <netedit/additionals/GNEDetectorExit.h>
#include <netedit/additionals/GNEStoppingPlace.h>
#include <netedit/additionals/GNERerouter.h>
#include <netedit/additionals/GNEVariableSpeedSign.h>
#include <netedit/frames/GNEInspectorFrame.h>
#include <netedit/GNEViewParent.h>

#include "GNEChange_Additional.h"

// ===========================================================================
// FOX-declarations
// ===========================================================================
FXIMPLEMENT_ABSTRACT(GNEChange_Additional, GNEChange, nullptr, 0)

// ===========================================================================
// member method definitions
// ===========================================================================

GNEChange_Additional::GNEChange_Additional(GNEAdditional* additional, bool forward) :
    GNEChange(additional->getViewNet()->getNet(), forward),
    myAdditional(additional),
    myLaneParent(nullptr),
    myEdgeParent(nullptr),
    myAdditionalParent(myAdditional->getAdditionalParent()),
    myEdgeChilds(myAdditional->getEdgeChilds()),
    myLaneChilds(myAdditional->getLaneChilds()) {
    assert(myNet);
    myAdditional->incRef("GNEChange_Additional");
    // handle additionals with lane parent
    if (GNEAttributeCarrier::hasAttribute(myAdditional->getTag(), SUMO_ATTR_LANE)) {
        myLaneParent = myNet->retrieveLane(myAdditional->getAttribute(SUMO_ATTR_LANE));
    }
    // handle additionals with edge parent
    if (GNEAttributeCarrier::hasAttribute(myAdditional->getTag(), SUMO_ATTR_EDGE)) {
        myEdgeParent = myNet->retrieveEdge(myAdditional->getAttribute(SUMO_ATTR_EDGE));
    }
}


GNEChange_Additional::~GNEChange_Additional() {
    assert(myAdditional);
    myAdditional->decRef("GNEChange_Additional");
    if (myAdditional->unreferenced()) {
        // show extra information for tests
        if (OptionsCont::getOptions().getBool("gui-testing-debug")) {
            WRITE_WARNING("Deleting unreferenced " + toString(myAdditional->getTag()) + " '" + myAdditional->getID() + "'");
        }
        // make sure that additional isn't in net before removing
        if (myNet->retrieveAdditional(myAdditional->getID(), false)) {
            myNet->deleteAdditional(myAdditional);
        }
        delete myAdditional;
    }
}


void
GNEChange_Additional::undo() {
    if (myForward) {
        // show extra information for tests
        if (OptionsCont::getOptions().getBool("gui-testing-debug")) {
            WRITE_WARNING("Removing " + toString(myAdditional->getTag()) + " '" + myAdditional->getID() + "' in GNEChange_Additional");
        }
        // delete additional of test
        myNet->deleteAdditional(myAdditional);
        // 1 - If additional own a lane parent, remove it from lane
        if (myLaneParent) {
            myLaneParent->removeAdditionalChild(myAdditional);
        }
        // 2 - If additional own a edge parent, remove it from edge
        if (myEdgeParent) {
            myEdgeParent->removeAdditionalChild(myAdditional);
        }
        // 3 - If additiona has a parent, remove it from their additional childs
        if (myAdditionalParent) {
            myAdditionalParent->removeAdditionalChild(myAdditional);
        }
        // 4 - if Additional has edge childs, remove it of their additional parents
        for (auto i : myEdgeChilds) {
            i->removeAdditionalParent(myAdditional);
        }
        // 5 - if Additional has lane childs, remove it of their additional parents
        for (auto i : myLaneChilds) {
            i->removeAdditionalParent(myAdditional);
        }
    } else {
        // show extra information for tests
        if (OptionsCont::getOptions().getBool("gui-testing-debug")) {
            WRITE_WARNING("Adding " + toString(myAdditional->getTag()) + " '" + myAdditional->getID() + "' in GNEChange_Additional");
        }
        // insert additional of test
        myNet->insertAdditional(myAdditional);
        // 1 - If additional own a Lane parent, add it to lane
        if (myLaneParent) {
            myLaneParent->addAdditionalChild(myAdditional);
        }
        // 2 - If additional own a edge parent, add it to edge
        if (myEdgeParent != nullptr) {
            myEdgeParent->addAdditionalChild(myAdditional);
        }
        // 3 - If additional has a parent, add it into additional parent
        if (myAdditionalParent) {
            myAdditionalParent->addAdditionalChild(myAdditional);
        }
        // 4 - if Additional has edge childs, add id into additional parents
        for (auto i : myEdgeChilds) {
            i->addAdditionalParent(myAdditional);
        }
        // 5 - if Additional has lane childs, add id into additional parents
        for (auto i : myLaneChilds) {
            i->addAdditionalParent(myAdditional);
        }
    }
    // Requiere always save additionals
    myNet->requiereSaveAdditionals();
    // check if inspector frame has to be updated
    if (myNet->getViewNet()->getViewParent()->getInspectorFrame()->shown()) {
        myNet->getViewNet()->getViewParent()->getInspectorFrame()->getACHierarchy()->refreshACHierarchy();
    }
}


void
GNEChange_Additional::redo() {
    if (myForward) {
        // show extra information for tests
        if (OptionsCont::getOptions().getBool("gui-testing-debug")) {
            WRITE_WARNING("Adding " + toString(myAdditional->getTag()) + " '" + myAdditional->getID() + "' in GNEChange_Additional");
        }
        // insert additional into net
        myNet->insertAdditional(myAdditional);
        // 1 - If additional own a Lane parent, add it to lane
        if (myLaneParent) {
            myLaneParent->addAdditionalChild(myAdditional);
        }
        // 2 - If additional own a edge parent, add it to edge
        if (myEdgeParent != nullptr) {
            myEdgeParent->addAdditionalChild(myAdditional);
        }
        // 3 - If additional has a parent, add it into additional parent
        if (myAdditionalParent) {
            myAdditionalParent->addAdditionalChild(myAdditional);
        }
        // 4 - if Additional has edge childs, add id into additional parents
        for (auto i : myEdgeChilds) {
            i->addAdditionalParent(myAdditional);
        }
        // 5 - if Additional has lane childs, add id into additional parents
        for (auto i : myLaneChilds) {
            i->addAdditionalParent(myAdditional);
        }
    } else {
        // show extra information for tests
        if (OptionsCont::getOptions().getBool("gui-testing-debug")) {
            WRITE_WARNING("Removing " + toString(myAdditional->getTag()) + " '" + myAdditional->getID() + "' in GNEChange_Additional");
        }
        // delete additional of test
        myNet->deleteAdditional(myAdditional);
        // 1 - If additional own a lane parent, remove it from lane
        if (myLaneParent) {
            myLaneParent->removeAdditionalChild(myAdditional);
        }
        // 2 - If additional own a edge parent, remove it from edge
        if (myEdgeParent) {
            myEdgeParent->removeAdditionalChild(myAdditional);
        }
        // 3 - If additiona has a parent, remove it from their additional childs
        if (myAdditionalParent) {
            myAdditionalParent->removeAdditionalChild(myAdditional);
        }
        // 4 - if Additional has edge childs, remove it of their additional parents
        for (auto i : myEdgeChilds) {
            i->removeAdditionalParent(myAdditional);
        }
        // 5 - if Additional has lane childs, remove it of their additional parents
        for (auto i : myLaneChilds) {
            i->removeAdditionalParent(myAdditional);
        }
    }
    // Requiere always save additionals
    myNet->requiereSaveAdditionals();
    // check if inspector frame has to be updated
    if (myNet->getViewNet()->getViewParent()->getInspectorFrame()->shown()) {
        myNet->getViewNet()->getViewParent()->getInspectorFrame()->getACHierarchy()->refreshACHierarchy();
    }
}


FXString
GNEChange_Additional::undoName() const {
    if (myForward) {
        return ("Undo create " + toString(myAdditional->getTag())).c_str();
    } else {
        return ("Undo delete " + toString(myAdditional->getTag())).c_str();
    }
}


FXString
GNEChange_Additional::redoName() const {
    if (myForward) {
        return ("Redo create " + toString(myAdditional->getTag())).c_str();
    } else {
        return ("Redo delete " + toString(myAdditional->getTag())).c_str();
    }
}
