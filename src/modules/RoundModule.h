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

#ifndef __ROUNDELIMINATION_ROUNDMODULE_H_
#define __ROUNDELIMINATION_ROUNDMODULE_H_

#include <omnetpp.h>
// #include "node.h"
#include <vector>
#include "utils.h"
#include "Rule.h"

using namespace omnetpp;

class Node;

/**
 * TODO - Generated class
 */
class RoundModule : public cSimpleModule
{
  private:
    int round; //!< current round
    int num_rounds; //!< total numbers of rounds
    int tact;
    state curState;
    std::vector<Node*> registeredNodes;
    int nrNodes;
    cXMLElement* schedule;

    cXMLElementList xml_rounds;

    // std::vector<Rule> cur_Select_Rules;
    // std::set<char,std::string> cur_Rename_Rules;

    bool verbose;



  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    /** \brief Performs selection step of current round on all registered nodes
     *
     * \return void
     */
    virtual void select();

    /** \brief Performs rename step of current round on all registered nodes
     *
     * \return void
     */
    virtual void rename();

    /** \brief Performs rename step of current round on all registered nodes
     *
     * \return void
     */
    virtual void weaken();

  public:
    bool registerToClock(Node* n);
};

#endif
