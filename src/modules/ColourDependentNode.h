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

#ifndef MODULES_COLOURDEPENDENTNODE_H_
#define MODULES_COLOURDEPENDENTNODE_H_

#include <omnetpp.h>
#include <vector>
#include <map>
#include "Rule.h"
#include "Label.h"
#include "utils.h"
#include "node.h"

using namespace omnetpp;

class Colour_dependent_node: public Node {
    private:
        bool WarnColorNotFund;
    protected:
        std::string cur_colour;
        std::map<std::string,std::string> colour_map;
        virtual void initialize(int stage) override;
        virtual int numInitStages() const override{
            return 2;
        }
        virtual void refreshDisplay() const override;
    public:
        virtual void step_select(const std::vector<Rule>& rules_to_check) override;
};

#endif /* MODULES_COLOURDEPENDENTNODE_H_ */
