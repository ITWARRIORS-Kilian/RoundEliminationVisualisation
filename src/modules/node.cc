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

#include "node.h"
#include "RoundModule.h"
#include "../messages/labelMessage_m.h"
#include "../channels/label_channel.h"

#define PRINTSELECT


Define_Module(Node);


void Node::initialize(int stage)
{
    switch(stage){
            case 0:
                round = 0;
                WATCH(round);
                cur_state = state::SELECT;
                // WATCH(cur_state);
                active = par("start_as_active").boolValue();
                WATCH(active);
                verbose = par("verbose").boolValue();
                clock = check_and_cast<RoundModule*>(getModuleByPath(par("clock_module").stringValue()));
                if (clock == nullptr) error("Clock module not found! Provided Path: %s",par("clock_module").stringValue());
                nrGates = gateSize("con");
                for(int i=0;i<nrGates;i++){
                    labels.emplace_back("");
                }
                WATCH(labels);
                verbose = par("verbose").boolValue();
                EV << "Init stage =" << stage << " success!\n";
            break;
            case 1:
                registered = clock->registerToClock(this);
                WATCH(registered);
                EV << "Init stage =" << stage << " success!\n";
//                if(active){
//                    LabelMessage* first_message = new LabelMessage("");
//                    first_message->setLabel("A");
//                    EV << "Send label Message" << "A" << "\n";
//                    send(first_message, "con$o", 0);
//                }
            break;
            default:
                EV << "Init stage =" << stage << " success!\n";
            break;
    }
}

void Node::handleMessage(cMessage *msg)
{
    if(dynamic_cast<LabelMessage*>(msg) != nullptr){
        LabelMessage* lmsg = check_and_cast<LabelMessage*>(msg);
        std::string label = lmsg->getLabel();
        int arrival_gate = msg->getArrivalGate()->getIndex();
        if(verbose) EV << "Received label: " << label << " on Gate:" << arrival_gate << std::endl;
        if(! labels.setLabel(arrival_gate,label)) error("Unable to set new label on gate %d/n",arrival_gate);
        //Add Label to inverse connection
        cChannel* invChannel = gate("con$o",arrival_gate)->findTransmissionChannel();
        if(invChannel == nullptr) throw cRuntimeError("Could not find Inverse gate");
        Label_channel* invlabelChannel = check_and_cast<Label_channel*>(invChannel);
        invlabelChannel->add_new_label(std::string(label));
    } else {
        if(verbose) EV << "Received unknown message type \n";
    }
}

void Node::step_select(const std::vector<Rule>& rules_to_check){
//    if(verbose){
        Enter_Method("SELECT");
//    }
//    else{
//        Enter_Method_Silent("SELECT");
//    }
    if(active){
        // This node is currently active ==> Perform selection step
        if(round==0){
            labels = rules_to_check[0];
            send_update();
        }
        else if (round>=1){
            ruleset result;
            int index_of_matching_rule = labels.check_all_permutations_set(rules_to_check, result);
            if(index_of_matching_rule == -1) error("No matching rule found for current labels");
//#ifdef PRINTSELECT
            Rule oldLabel = labels;
//#endif
//            labels = result.first.select_following_Rule(rules_to_check[index_of_matching_rule], result.second);
            labels.select_following_Rule(rules_to_check[index_of_matching_rule], result.second);
//#ifdef PRINTSELECT
                EV << oldLabel <<" matches rule "<<rules_to_check[index_of_matching_rule] << " in form " << result << " ==> " << labels << std::endl;
//#endif
            send_update();
        }
        else{
            //This is not a valid round number ==> Error
            error("Invalid round number in select step, round=%d",round);
        }
    }
}

//void Node::step_weaken(int round){
//
//}

void Node::step_rename(std::map<char,std::string>& rename_rule){
    Enter_Method("Rename");
    if(active){
        labels.rename(rename_rule);
        send_update();
    }
}

void Node::step_switch(){
    Enter_Method("SWITCH");
    active = !active;
    if(verbose) EV << "Switch to active=" << active << "\n";
    round++;
}

void Node::send_update(){
    for(int i=0; i< nrGates; i++){
        std::string label_out = labels[i].GetLabels_asString();
        LabelMessage* message = new LabelMessage(label_out.c_str());
        message->setLabel(label_out.c_str());
        EV << "Send label Message " << label_out << " on Gate " << i << "\n";
        send(message, "con$o", i);
    }
}
