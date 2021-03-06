/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2018 German Aerospace Center (DLR) and others.
// This program and the accompanying materials
// are made available under the terms of the Eclipse Public License v2.0
// which accompanies this distribution, and is available at
// http://www.eclipse.org/legal/epl-v20.html
// SPDX-License-Identifier: EPL-2.0
/****************************************************************************/
/// @file    MSJunction.cpp
/// @author  Christian Roessel
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @date    Wed, 12 Dez 2001
/// @version $Id$
///
// The base class for an intersection
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include "MSVehicle.h"
#include "MSEdge.h"
#include "MSJunction.h"


// ===========================================================================
// class declarations
// ===========================================================================
class MSLink;

// ===========================================================================
// static member definitions
// ===========================================================================

// ===========================================================================
// member method definition
// ===========================================================================
MSJunction::MSJunction(const std::string& id, SumoXMLNodeType type, const Position& position,
                       const PositionVector& shape) :
    Named(id),
    myType(type),
    myPosition(position),
    myShape(shape) {
}


MSJunction::~MSJunction() {}


const Position&
MSJunction::getPosition() const {
    return myPosition;
}


void
MSJunction::postloadInit() {}


int
MSJunction::getNrOfIncomingLanes() const {
    int nr = 0;
    for (const MSEdge* e : myIncoming) {
        nr += (int)e->getLanes().size();
    }
    return nr;
}


void
MSJunction::passedJunction(const MSVehicle* vehicle) {
    myLinkLeaders.erase(vehicle);
}


bool
MSJunction::isLeader(const MSVehicle* ego, const MSVehicle* foe) {
    if (foe->getLane()->getEdge().getToJunction() != this) {
        // foe is already past the junction so is definitely a leader
        return true;
    }
    if (myLinkLeaders.find(ego) == myLinkLeaders.end() || myLinkLeaders[ego].count(foe) == 0) {
        // we are not yet the leader for foe, thus foe will be our leader
        myLinkLeaders[foe].insert(ego);
        return true;
    } else {
        return false;
    }
}
/****************************************************************************/

