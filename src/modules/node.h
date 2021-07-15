//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
//  _  _ ___ _     ___    _    _   _   _____ ____  ____  _____ _   _
// | |/ /_ _| |   |_ _|  / \  | \ | | | ____|  _ \| __ )| ____| \ | |
// | ' / | || |    | |  / _ \ |  \| | |  _| | |_) |  _ \|  _| |  \| |
// | . \ | || |___ | | / ___ \| |\  | | |___|  _ <| |_) | |___| |\  |
// |_|\_\___|_____|___/_/   \_\_| \_| |_____|_| \_\____/|_____|_| \_|
// 
// Author: Kilian Erben
// Email: kilian.erben@gmail.com

#ifndef __ROUNDELIMINATION_NODE_H_
#define __ROUNDELIMINATION_NODE_H_

#include <omnetpp.h>
#include <vector>
#include "Rule.h"
#include "Label.h"
#include "utils.h"
// #include "RoundModule.h"

using namespace omnetpp;

class RoundModule;

/**
 * Class representing the functionality of a node using the round elimination based algorithms
 */
class Node : public cSimpleModule
{
  private:

  protected:
    int round; //stores the current round the module is in
    state cur_state; //stores in which sub-step the module is currently in
    bool active; // true if module is currently active(true) or passive(false)
    RoundModule* clock; // Round Module from which the node gets the current round and state
    bool registered; //true if registered to a RoundModule
    Rule labels; // stores the labels of all incident edges
    int nrGates; //number of incident edges
    bool verbose; // if true additional informations are displayed in the simulation

    virtual void initialize(int stage) override;
    virtual int numInitStages() const override{
        return 2;
    }
    virtual void handleMessage(cMessage *msg) override;

    /** \brief Send currently stored labels to the corresponding gates
     *
     */
    void send_update();
  public:
    virtual void step_select(const std::vector<Rule>& rules_to_check);
    virtual void step_rename(std::map<char,std::string>& rename_rule, bool sendUpdate = true);
    virtual void step_switch();
};

#endif
