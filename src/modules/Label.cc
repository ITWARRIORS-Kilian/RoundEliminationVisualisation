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

#include "Label.h"

Label::Label()
{
    num_labels = 0;
    //ctor
}

Label::Label(std::string label)
{
    num_labels = 0;
    InsertNewLabels(label);
}

Label::~Label()
{
    //dtor
}

void Label::InsertNewLabels(std::string label)
{
    for(unsigned int i=0; i<label.length(); i++){
        labels.insert(label[i]);
        num_labels++;
    }
}

bool Label::operator ==(const char label) const{
    if(labels.find(label) == labels.end()) return false;
    else return true;
}

bool Label::operator ==(const std::string label) const{
    for(unsigned int i=0; i<label.length(); i++){
        if(labels.find(label[i]) != labels.end()) return true;
    }
    return false;
}

bool Label::operator ==(const Label& label) const{
    const std::set<char>& rh_labels = label.GetLabels();
    for(auto elem : rh_labels){
        if(labels.find(elem) != labels.end()) return true;
    }
    return false;
}

bool Label::operator <(const char label){
    if( *(labels.begin()) < label) return true;
    else return false;
}

bool Label::operator <(const std::string label){
    if(*(labels.begin()) < *(label.begin())) return true;
    else return false;
}

bool Label::operator <(const Label& label) const{
    const std::set<char>& rh_labels = label.GetLabels();
    if(*(labels.begin()) < *(rh_labels.begin())) return true;
    else return false;
}

bool Label::operator <(const Label* label){
    const std::set<char>& rh_labels = label->GetLabels();
    if(*(labels.begin()) < *(rh_labels.begin())) return true;
    else return false;
}

char Label::select(const Label& label) const{
    const std::set<char>& rh_labels = label.GetLabels();
    for(auto elem : rh_labels){
        auto index = labels.find(elem);
        if(index != labels.end()) return *index;
    }
    throw omnetpp::cRuntimeError("Label do not match");
    return (char) 0;
}

void Label::rename(std::map<char,std::string>& rename_rules){
    std::set<char> labels_copy = labels;
    labels.clear();
    for(char c:labels_copy){
        if(rename_rules.find(c) == rename_rules.end()) throw omnetpp::cRuntimeError("no Fitting RenameRule for %c", c);
        InsertNewLabels(rename_rules[c]);
    }
}


#ifndef output_operator
#define output_operator
std::ostream& operator << (std::ostream& os, const Label& label){
    for(char elem:label.GetLabels()){
        os << elem;
    }
    return os;
}
#endif // output_operator


