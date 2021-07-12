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

package roundelimination.networks;

import roundelimination.channels.Label_channel;
import roundelimination.modules.Colour_dependent_black_node;
import roundelimination.modules.Node;
import roundelimination.modules.Colour_dependent_white_node;
import roundelimination.modules.RoundModule;

network simpleMMNetworkColored
{
    @display("bgb=1016.25,703.75");
    submodules:

        n2b: Colour_dependent_black_node {
            @display("p=186.25,153.75");
        }
        n0b: Colour_dependent_black_node {
            @display("p=437.5,46.25");
        }
        n5b: Colour_dependent_black_node {
            @display("p=767.5,153.75");
        }
        n7b: Colour_dependent_black_node {
            @display("p=321.25,276.25");
        }
        n8b: Colour_dependent_black_node {
            @display("p=573.75,276.25");
        }
        n11b: Colour_dependent_black_node {
            @display("p=186.25,407.5");
        }
        n12b: Colour_dependent_black_node {
            @display("p=458.75,465");
        }
        n13b: Colour_dependent_black_node {
            @display("p=767.5,426.25");
        }
        n17b: Colour_dependent_black_node {
            @display("p=321.25,642.5");
        }
        n3w: Colour_dependent_white_node {
            @display("p=321.25,153.75");
        }
        n4w: Colour_dependent_white_node {
            @display("p=573.75,153.75");
        }
        n1w: Colour_dependent_white_node {
            @display("p=872.5,46.25");
        }
        n9w: Colour_dependent_white_node {
            @display("p=767.5,276.25");
        }
        n10w: Colour_dependent_white_node {
            @display("p=458.75,371.25");
        }
        n6w: Colour_dependent_white_node {
            @display("p=186.25,276.25");
        }
        n14w: Colour_dependent_white_node {
            @display("p=46.25,407.5");
        }
        n15w: Colour_dependent_white_node {
            @display("p=316.25,518.75");
        }
        n16w: Colour_dependent_white_node {
            @display("p=622.5,518.75");
        }
        clock: RoundModule {
            @display("p=921.25,233.75");
        }
    connections:

        n4w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n5b.con++;
        n4w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n8b.con++;
        n8b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n9w.con++;
        n5b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n9w.con++;
        n5b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n1w.con++;
        n0b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n1w.con++;
        n0b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n4w.con++;
        n3w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n0b.con++;
        n2b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n3w.con++;
        n2b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n6w.con++;
        n3w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n7b.con++;
        n6w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n7b.con++;
        n7b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n10w.con++;
        n10w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n8b.con++;
        n10w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n12b.con++;
        n12b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n16w.con++;
        n12b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n15w.con++;
        n16w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n13b.con++;
        n13b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n9w.con++;
        n15w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n17b.con++;
        n17b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n16w.con++;
        n6w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n11b.con++;
        n11b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n15w.con++;
        n11b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n14w.con++;
        n14w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n17b.con++;
        n2b.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n14w.con++;
        n1w.con++ <--> Label_channel{color_edge=true;schedule = xmldoc("../simulations/schedules/schedule2.xml");} <--> n13b.con++;
}
