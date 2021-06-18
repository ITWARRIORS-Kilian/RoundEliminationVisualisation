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

#include "utils.h"

state state_step_forward(state& st){
    switch(st){
        case state::SELECT:
            return state::WEAKEN;
        break;
        case state::WEAKEN:
            return state::RENAME;
        break;
        case state::RENAME:
            return state::SWITCH;
        break;
        case state::SWITCH:
            return state::SELECT;
        break;
        default:
            throw omnetpp::cRuntimeError("state enum reached invalid state");
    }
}

std::ostream& operator<<(std::ostream& os, const state& st)
{
    switch(st){
        case state::SELECT:
            os <<"SELECT";  // no endl!
        break;
        case state::WEAKEN:
            os <<"WEAKEN";  // no endl!
        break;
        case state::RENAME:
            os <<"RENAME";  // no endl!
        break;
        case state::SWITCH:
            os <<"SWITCH";  // no endl!
        break;
    }
    return os;
}
