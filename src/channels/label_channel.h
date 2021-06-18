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

#ifndef __ROUNDELIMINATION_LABEL_CHANNEL_H_
#define __ROUNDELIMINATION_LABEL_CHANNEL_H_

#include <omnetpp.h>
#include <iostream>
#include <vector>

using namespace omnetpp;

/**
 * TODO - Generated cDatarateChannel
 */
class Label_channel : public cDatarateChannel
{
  protected:
//    bool isTransmissionChannel() const;
//    simtime_t getTransmissionFinishTime() const;
	void processMessage(cMessage *msg, simtime_t t, result_t& result) override;
	virtual void initialize() override;
	virtual void refreshDisplay() const override;
  private:
	simtime_t delay; // propagation delay
    double datarate; // data rate
    double ber;      // bit error rate
    double per;      // packet error rate
    // int show_back;   //!< defines how many rounds later the message

    char colour[10];
	char label[20]; //Contains label
	std::vector<std::string> old_labels;
	bool verbose;

	enum {
      FL_ISDISABLED = 1 << 10,
      FL_DELAY_NONZERO = 1 << 11,
      FL_DATARATE_NONZERO = 1 << 12,
      FL_BER_NONZERO = 1 << 13,
      FL_PER_NONZERO = 1 << 14,
    };

	simtime_t txFinishTime;

  public:
       /** \brief add new label to list and update current label
       *  This function should be called from the inverse transmission channel to update the labels of both channels
       * \param std::string
       * \return void
       */
      void add_new_label(std::string new_label){
          old_labels.push_back(new_label);
          snprintf(label,20,"%s",new_label.c_str());
          snprintf(colour,20,"#0000FF");
      }


};

#endif
