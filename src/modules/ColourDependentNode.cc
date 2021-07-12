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
//_  _ ___ _     ___    _    _   _   _____ ____  ____  _____ _   _
// | |/ /_ _| |   |_ _|  / \  | \ | | | ____|  _ \| __ )| ____| \ | |
// | ' / | || |    | |  / _ \ |  \| | |  _| | |_) |  _ \|  _| |  \| |
// | . \ | || |___ | | / ___ \| |\  | | |___|  _ <| |_) | |___| |\  |
// |_|\_\___|_____|___/_/   \_\_| \_| |_____|_| \_\____/|_____|_| \_|
// 
// Author: Kilian Erben
// Email: kilian.erben@gmail.com


#include "ColourDependentNode.h"
#include "node.h"

Define_Module(Colour_dependent_node);

void Colour_dependent_node::initialize(int stage)
{
    // init base class
    Node::initialize(stage);
    // init additional members
    if (stage == 0){
        cur_colour = par("colour").stdstringValue();
        WATCH(cur_colour);
        cXMLElement* colourTableXML = par("colourTable").xmlValue();
        cXMLElementList colorRulesXML = colourTableXML->getChildrenByTagName("colorRule");
        for(cXMLElement* colorRuleXML:colorRulesXML){
            if(colorRuleXML->getAttribute("id") == nullptr) error("colorRule needs attribute id");
            std::string colourRule_id = colorRuleXML->getAttribute("id");
            if(colorRuleXML->getAttribute("color") == nullptr) error("colorRule needs attribute color");
            std::string colourRule_colour = colorRuleXML->getAttribute("color");
            colour_map.emplace(colourRule_id,colourRule_colour);
        }
        if(verbose) EV << "Loaded " << colorRulesXML.size() << "coloring rules from xml coloring table" << std::endl;
        WarnColorNotFund = par("WarnColorNotFund").boolValue();
    }
}

void Colour_dependent_node::step_select(const std::vector<Rule>& rules_to_check){
    #if !defined(ENTER_METHOD_SILENT_NODE)
        Enter_Method("SELECT");
    #else
        Enter_Method_Silent("SELECT");
    #endif
    if(active){
        // This node is currently active ==> Perform selection step
        if(round==0){
            std::string color = "";
            bool found=false;
            for(Rule rule:rules_to_check){
                if(rule.get_Colour(color)){
                    EV << color << " comp  " << cur_colour << std::endl;
                    if(color==cur_colour){
                        //found correct rule
                        labels = rule;
                        found = true;
                        break;
                    }
                }
            }
            if(not found){
                if(WarnColorNotFund) error("No matching color found in first round");
                labels = rules_to_check[0];
                EV << "Found no matching Select Rule in first round (color does not match color of any rule) ==> Select first rule as default" << std::endl;
            }
            send_update();
        }
        else if (round>=1){
            ruleset result;
            int index_of_matching_rule = labels.check_all_permutations_set(rules_to_check, result);
            if(index_of_matching_rule == -1) error("No matching rule found for current labels");
            Rule oldLabel = labels;
            labels.select_following_Rule(rules_to_check[index_of_matching_rule], result.second);
            if(verbose){
                EV << oldLabel <<" matches rule "<<rules_to_check[index_of_matching_rule] << " in form " << result << " ==> " << labels << std::endl;
            }
            //update colour
            if(rules_to_check[index_of_matching_rule].get_Colour(cur_colour) and verbose){
                EV << "Updated color: " << cur_colour << std::endl;
            }
            //send decision to neighbors
            send_update();
        }
        else{
            //This is not a valid round number ==> Error
            error("Invalid round number in select step, round=%d",round);
        }
    }
}

void Colour_dependent_node::refreshDisplay() const{
    //get colour
    char buf[50] = {0};
    //snprintf(buf, 50, cur_colour.c_str());
    //getDisplayString().setTagArg("t", 0, buf);
    auto iter_colour_find = colour_map.find(cur_colour);
    if(iter_colour_find != colour_map.end()){
        snprintf(buf, 50, iter_colour_find->second.c_str());
        getDisplayString().setTagArg("i", 1, buf);
    }
    else error("Could not find color in color table");
}
