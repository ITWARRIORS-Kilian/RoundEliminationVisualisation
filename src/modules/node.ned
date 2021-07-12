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

package roundelimination.modules;

//
// Nodes that are able to perform the round elimination Algorithm
//
simple Node
{
    parameters:
        @class(Node);
        string colour = default("white"); // colour of node
        int id = default(1); // unique identifier
        bool start_as_active = default(true); // defines if node starts in active mode
        string clock_module = default("^.clock"); // absolut or relativ path to clock module
		xml shedule = default(xml("<root/>"));
		
		bool verbose = default(true); // If True the node will generate additional output
        string text_pos = default("t"); // If verbose == true : parameter defines where the additional info is displayed. Possible values: t(top), r(right), l(left)
        @display("i=misc/node2,$colour;t=,$text_pos");
    gates:
        inout con[]; // inout connection to genecrically connect to other nodes
        
}

simple Black_node extends Node //still uses the implementation of Node
{
    colour = "black";
    start_as_active = false;
}

simple White_node extends Node //still uses the implementation of Node
{
    colour = "white";
    start_as_active = true;
}

simple Colour_dependent_node extends Node
{
    @class(Colour_dependent_node);
    colour = default("1");
    xml colourTable = default(xml("<colorTable></colorTable>"));
    bool WarnColorNotFund = default(true); //if true a warning is thrown if par colour does not match any colour in the selection rules specified in par shedule; if false node uses first rule
    @display("i=misc/node2;t=,$text_pos");
}

simple Colour_dependent_black_node extends Colour_dependent_node //still uses the implementation of Node
{
    colour = "black";
    start_as_active = false;
}

simple Colour_dependent_white_node extends Colour_dependent_node //still uses the implementation of Node
{
    colour = "white";
    start_as_active = true;
}
