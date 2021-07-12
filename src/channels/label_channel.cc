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
//  _  _ ___ _     ___    _    _   _   _____ ____  ____  _____ _   _
// | |/ /_ _| |   |_ _|  / \  | \ | | | ____|  _ \| __ )| ____| \ | |
// | ' / | || |    | |  / _ \ |  \| | |  _| | |_) |  _ \|  _| |  \| |
// | . \ | || |___ | | / ___ \| |\  | | |___|  _ <| |_) | |___| |\  |
// |_|\_\___|_____|___/_/   \_\_| \_| |_____|_| \_\____/|_____|_| \_|
// 
// Author: Kilian Erben
// Email: kilian.erben@gmail.com

#include "label_channel.h"
#include "../modules/RoundModule.h"
#include "../messages/labelMessage_m.h"

Define_Channel(Label_channel);

void Label_channel::initialize(int stage){
    switch (stage) {
        case 0:
            verbose = par("verbose").boolValue();
            WATCH(verbose);
            for(int i=0;i<20;i++){
                label[i]=0;
            }
            snprintf(label,20,"Unlabeled");
            snprintf(colour_label,20,"#0000FF");
            old_labels.emplace_back(std::string(label));
            WATCH(label);

            color_edge = par("color_edge").boolValue();
            WATCH(color_edge);
            if(color_edge){
                //load schedule
                round = 0;
                // Parse XML File
                cXMLElement* schedule = par("schedule").xmlValue();
                xml_rounds = schedule->getChildrenByTagName("Round");
                num_rounds = xml_rounds.size();
                WATCH(num_rounds);
                cur_color = "#000000";
                WATCH(cur_color);
                // if(num_rounds == 0) EV << "XML Schedule does not contain any rounds" << std::endl;
            }

            delay = par("delay");
            datarate = par("datarate");
            ber = par("ber");
            per = par("per");

            if (delay < SIMTIME_ZERO)
                throw cRuntimeError(this, "Negative delay %s", SIMTIME_STR(delay));
            if (datarate < 0)
                throw cRuntimeError(this, "Negative datarate %g", datarate);
            if (ber < 0 || ber > 1)
                throw cRuntimeError(this, "Wrong bit error rate %g", ber);
            if (per < 0 || per > 1)
                throw cRuntimeError(this, "Wrong packet error rate %g", per);

            setFlag(FL_ISDISABLED, par("disabled"));
            setFlag(FL_DELAY_NONZERO, delay != SIMTIME_ZERO);
            setFlag(FL_DATARATE_NONZERO, datarate != 0);
            setFlag(FL_BER_NONZERO, ber != 0);
            setFlag(FL_PER_NONZERO, per != 0);
        break;
        case 1:
            //register to clock
            if(color_edge){
                RoundModule* clock = check_and_cast<RoundModule*>(getParentModule()->getModuleByPath(par("clock_module").stringValue()));
                if (clock == nullptr) throw cRuntimeError("Clock module not found! Provided Path: %s",par("clock_module").stringValue());
                if (!clock->registerToClock(this)) throw cRuntimeError("Unable to register to clock");
            }
        break;
        default:

        break;
    }

}

//bool Label_channel::isTransmissionChannel() const{
//    return true;
//}
//
//simtime_t Label_channel::getTransmissionFinishTime() const{
//    return txfinishtime;
//}
//
void Label_channel::processMessage(cMessage *msg, simtime_t t, result_t& result){
    // if channel is disabled, signal that message should be deleted
        if (flags & FL_ISDISABLED) {
            result.discard = true;
            return;
        }

        // datarate modeling
        if ((flags & FL_DATARATE_NONZERO) && msg->isPacket()) {
            cPacket *pkt = (cPacket *)msg;
            simtime_t duration = pkt->getBitLength() / datarate;
            result.duration = duration;
            txFinishTime = t + duration;
        }
        else {
            txFinishTime = t;
        }

        //get label from message
        if(dynamic_cast<LabelMessage*>(msg) != nullptr){
                LabelMessage* lmsg = check_and_cast<LabelMessage*>(msg);
                strncpy(label,lmsg->getLabel(),20);
                old_labels.emplace_back(std::string(label));
                if(colorRules.find(std::string(label))!=colorRules.end()){
                    cur_color=colorRules[std::string(label)];
                }
                if(verbose) EV << "Received label Message" << label << "\n";
                switch_label_colour();
        }

        // propagation delay modeling
        result.delay = delay;
}

void Label_channel::refreshDisplay() const
{
    if(verbose){
        // refresh Display String
        char buf[20] = {0};
//        snprintf(buf,20,"%s",label);
        snprintf(buf,20,"%s",(*(old_labels.end()-1)).c_str());
        getDisplayString().setTagArg("t", 0, buf);
        getDisplayString().setTagArg("t", 2, colour_label);
        snprintf(buf, 20,cur_color.c_str());
        getDisplayString().setTagArg("ls", 0, buf);

    }
}

void Label_channel::update_coloring_rules(int Round){
    Enter_Method_Silent("Update coloring rules!");
    round = Round;
    cXMLElement* xml_cur_round = xml_rounds[round];
    cXMLElement* xml_cur_colorRules = xml_cur_round->getFirstChildWithTag("colorRules");
    if(xml_cur_colorRules == nullptr) throw cRuntimeError("Found no colorRules Tag in round %d",round);
    cXMLElementList xml_colorEdgeRules = xml_cur_colorRules->getChildrenByTagName("ColorEdgeRule");
    for(cXMLElement* xml_colorEdgeRule:xml_colorEdgeRules){
        if(xml_colorEdgeRule->getAttribute("label") == nullptr) throw cRuntimeError("Found ColorEdgeRule without attribute label");
        std::string key =  xml_colorEdgeRule->getAttribute("label");
        if(xml_colorEdgeRule->getAttribute("color") == nullptr) throw cRuntimeError("Found ColorEdgeRule without attribute color");
        std::string colortokey = xml_colorEdgeRule->getAttribute("color");
        colorRules[key]=colortokey;
    }
}
