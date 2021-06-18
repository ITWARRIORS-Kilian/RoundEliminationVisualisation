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


#include "RoundModule.h"
#include "node.h"
#include "utils.h"
#include <vector>

Define_Module(RoundModule);

void RoundModule::initialize()
{
    tact = par("tact").intValue();
    round = 0;
    WATCH(round);
    curState = state::SELECT;
    WATCH(curState);
    verbose = par("verbose").boolValue();
    nrNodes = 0;
    WATCH(nrNodes);
    schedule = par("schedule").xmlValue();//->getFirstChildWithTag("schedule");
    // WATCH(schedule); Zeigt nur Pointer an

    // Parse XML File
    xml_rounds = schedule->getChildrenByTagName("Round");
    num_rounds = xml_rounds.size();
    WATCH(num_rounds);
    if(num_rounds == 0) EV << "XML Schedule does not contain any rounds" << std::endl;

    // Start with first round
    scheduleAt(tact, new cMessage("Tic",1));
}

void RoundModule::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage()){
        EV << "Round:"<< round << " Step:"<< curState << std::endl;
        switch(curState){
        case state::SELECT:
            select();
        break;
        case state::WEAKEN:
            weaken();
        break;
        case state::RENAME:
            rename();
        break;
        case state::SWITCH:
            for(Node* node:registeredNodes){
                node->step_switch();
            }
            round ++;
        break;
        default:
            error("Clock is in invalid State!");
        }
        curState = state_step_forward(curState);
        // if(curState == state::SWITCH) round++;
        if(round < num_rounds){
            scheduleAt(simTime()+tact, msg);
        }
    }
}

bool RoundModule::registerToClock(Node* n){
    Enter_Method_Silent();
    registeredNodes.push_back(n);
    nrNodes++;
    return true;
}

void RoundModule::select(){
    // Load current rules
    cXMLElement* xml_cur_round = xml_rounds[round];
    cXMLElement* xml_cur_select = xml_cur_round->getFirstChildWithTag("select");
    if(xml_cur_select == nullptr) error("Found no select tag in round %d",round);
    cXMLElementList xml_SelectRules = xml_cur_select->getChildrenByTagName("SelectRule");
    if(xml_SelectRules.size() == 0) error("Found no SelectRule in round %d",round);
    std::vector<Rule> cur_Select_Rules;
    cur_Select_Rules.clear();
    for(cXMLElement* xml_SelectRule:xml_SelectRules){
        if(xml_SelectRule->getAttribute("rule") == nullptr) error("SelectRule has no attribute rule");
        std::string rule_string = xml_SelectRule->getAttribute("rule");
        cur_Select_Rules.emplace_back(rule_string);
    }
    if(verbose) EV << "Loaded " << cur_Select_Rules.size() << " SelectRules\n";
    // Perform Selection Step on all registered nodes
    for(Node* node:registeredNodes){
        node->step_select(cur_Select_Rules);
    }
}

void RoundModule::rename(){
    cXMLElement* xml_cur_round = xml_rounds[round];
    cXMLElement* xml_cur_rename = xml_cur_round->getFirstChildWithTag("rename");
    if(xml_cur_rename == nullptr) error("Found no rename tag in round %d",round);
    cXMLElementList xml_RenameRules = xml_cur_rename->getChildrenByTagName("RenameRule");
    if(xml_RenameRules.size() == 0) error("Found no RenameRule in round %d",round);
    std::map<char,std::string> cur_Rename_Rules;
    cur_Rename_Rules.clear();
    for(cXMLElement* xml_RenameRule:xml_RenameRules){
        if(xml_RenameRule->getAttribute("old") == nullptr) error("A RenameRule has no attribute old");
        if(xml_RenameRule->getAttribute("new") == nullptr) error("A RenameRule has no attribute old");
        std::string old_string = xml_RenameRule->getAttribute("old");
        std::string new_string = xml_RenameRule->getAttribute("new");
        if(old_string.length() != 1) error("Each RenameRule should only contain one old label");
        if(new_string.length() < 1) error("Each RenameRule should contain at least one new label");
        char c = old_string[0];
        cur_Rename_Rules[c] = new_string;
    }
    if(verbose) EV << "Loaded " << cur_Rename_Rules.size() << " RenameRules\n";
    for(Node* node:registeredNodes){
        node->step_rename(cur_Rename_Rules);
    }
}

void RoundModule::weaken(){
    //Load rules
    cXMLElement* xml_cur_round = xml_rounds[round];
    cXMLElement* xml_cur_weaken = xml_cur_round->getFirstChildWithTag("weaken");
    if(xml_cur_weaken == nullptr){
        if(verbose) EV << "Found no weaken tag in round " << round << ". Assume this means no weakening is required in this Round.\n";
        return;
    }
    cXMLElementList xml_RenameRules = xml_cur_weaken->getChildrenByTagName("WeakenRenameRule");
    cXMLElementList xml_SelectRules = xml_cur_weaken->getChildrenByTagName("WeakenSelectRule");
    if(xml_RenameRules.size() == 0 and xml_SelectRules.size() == 0) {
        if(verbose) EV << "Found no WeakenRenameRule nor WeakenSelectRule in weaken tag in round " << round << ". Assume this means no weakening is required in this Round.\n";
        return;
    }
    else if(xml_RenameRules.size() == 0 and xml_SelectRules.size() == 0) error("Each weaken tag requires at least one WeakenSelectRule and one WeakenRenameRule. Round %d \n",round);

    // Load Rename rules and insert into map
    std::map<char,std::string> cur_Rename_Rules;
    cur_Rename_Rules.clear();
    for(cXMLElement* xml_RenameRule:xml_RenameRules){
        if(xml_RenameRule->getAttribute("old") == nullptr) error("A WeakenRenameRule has no attribute old");
        if(xml_RenameRule->getAttribute("new") == nullptr) error("A WeakenRenameRule has no attribute old");
        std::string old_string = xml_RenameRule->getAttribute("old");
        std::string new_string = xml_RenameRule->getAttribute("new");
        if(old_string.length() != 1) error("Each WeakenRenameRule should only contain one old label");
        if(new_string.length() < 1) error("Each WeakenRenameRule should contain at least one new label");
        char c = old_string[0];
        cur_Rename_Rules[c] = new_string;
    }
    if(verbose) EV << "Loaded " << cur_Rename_Rules.size() << " WeakenRenameRules\n";

    // Load Weaken rules and insert into rule vector
    std::vector<Rule> cur_Select_Rules;
    cur_Select_Rules.clear();
    for(cXMLElement* xml_SelectRule:xml_SelectRules){
        if(xml_SelectRule->getAttribute("rule") == nullptr) error("WeakenSelectRule has no attribute rule");
        std::string rule_string = xml_SelectRule->getAttribute("rule");
        cur_Select_Rules.emplace_back(rule_string);
    }
    if(verbose) EV << "Loaded " << cur_Select_Rules.size() << " WeakenSelectRules\n";

    for(Node* node:registeredNodes){
        node->step_rename(cur_Rename_Rules);
        node->step_select(cur_Select_Rules);
    }

}
