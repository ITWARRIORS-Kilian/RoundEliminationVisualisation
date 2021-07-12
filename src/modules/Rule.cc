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

#include "Rule.h"
#include <algorithm>
#include <sstream>
#include <iostream>

Rule::Rule()
{
    size = 0;
    is_coloured = false;
    colour = "Not a colour ==> This rule has no color";
}

Rule::Rule(std::string rule_string){
    size = 0;
    std::istringstream rule_stream(rule_string);
    std::string subs;
    while(std::getline(rule_stream, subs,' ')){
        size++;
        rule.emplace_back(subs);
    }
    is_coloured = false;
    colour = "Not a colour ==> This rule has no color";
}

Rule::Rule(std::string rule_string, std::string Colour){
    size = 0;
    std::istringstream rule_stream(rule_string);
    std::string subs;
    while(std::getline(rule_stream, subs,' ')){
        size++;
        rule.emplace_back(subs);
    }
    is_coloured = true;
    colour = Colour;
}

Rule::~Rule()
{
    //dtor
}

void Rule::Get_All_Permutation(std::vector<ruleset>& out){
    std::vector<int> index_permutation;
    for(int i=0;i<size;i++){
        index_permutation.push_back(i);
    }
    do{
        Rule label_permutation;
        for(int index:index_permutation){
            label_permutation.push_back(rule[index]);
        }
        out.push_back(ruleset(label_permutation,index_permutation));
    }while(next_permutation(index_permutation.begin(),index_permutation.end()));

}

std::ostream& operator<<(std::ostream& os, const Rule& rule){
    for(Label elem:rule.rule){
        os << elem <<" ";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const ruleset& ruleset_p){
    os << ruleset_p.first <<" (";
    for(int i:ruleset_p.second){
        os << i<<",";
    }
    os << ")";
    return os;
}

const Label& Rule::operator[](unsigned int i) const{
    return rule[i];
}

bool Rule::operator==(const Rule& rh_rule) const{
    for(int i=0;i<size;i++){
        if(!(rh_rule[i] == rule[i])) return false;
    }
    return true;
}

bool Rule::check_all_permutations(const Rule& rule_to_check, ruleset& found_perm) const{
    std::vector<int> index_permutation;
    for(int i=0;i<size;i++){
        index_permutation.push_back(i);
    }
    do{
        Rule label_permutation;
        for(int index:index_permutation){
            label_permutation.push_back(rule[index]);
        }
        if(label_permutation == rule_to_check){
            found_perm = ruleset(label_permutation,index_permutation);
            return true;
        }
    }while(next_permutation(index_permutation.begin(),index_permutation.end()));
    return false;
}

int Rule::check_all_permutations_set(const std::vector<Rule>& rules_to_check, ruleset& found_perm) const{
    std::vector<int> index_permutation;
    for(int i=0;i<size;i++){
        index_permutation.push_back(i);
    }
    do{
        Rule label_permutation;
        for(int index:index_permutation){
            label_permutation.push_back(rule[index]);
        }
        for(unsigned int j=0; j < rules_to_check.size(); j++){
            if(label_permutation == rules_to_check[j]){
                found_perm = ruleset(label_permutation,index_permutation);
                return j;
            }
        }
    }while(next_permutation(index_permutation.begin(),index_permutation.end()));
    return -1;
}

void Rule::select_following_Rule(const Rule& selection_rule, std::vector<int> order){
//    for(unsigned int i:order){
//        char sel = rule[i].select(selection_rule[i]);
//        if(sel>=0) ret.emplace_back(std::string(1,sel));
//        else{
//            // This means set does not match rule ==> should not happen
//            throw omnetpp::cRuntimeError("Labels do not match Rule");
//            std::vector<char> error_out = {};
//        }
//    }
    for(int i=0;i<size;i++){
        int order_index = order[i];
        char sel = rule[order_index].select(selection_rule[i]);
        if(sel>=0){
            if (! setLabel(order_index, std::string(1,sel))) throw omnetpp::cRuntimeError("Could not set new Label");
        }
        else{
            // This means set does not match rule ==> should not happen
            throw omnetpp::cRuntimeError("Labels do not match Rule");
            std::vector<char> error_out = {};
        }
    }
}


