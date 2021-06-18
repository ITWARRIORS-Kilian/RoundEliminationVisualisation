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

#ifndef MODULES_LABEL_H_
#define MODULES_LABEL_H_

#include <set>
#include <iostream>
#include <omnetpp.h>

class Label {
public:
    Label();
    Label(std::string label);
    virtual ~Label();

    /** Access num_labels
     * \return The current value of num_labels
     */
    unsigned int Getnum_labels() { return num_labels; }
    /** Set num_labels
     * \param val New value to set
     */
    void Setnum_labels(unsigned int val) { num_labels = val; }
    /** Insert new labels by splitting a std::string into it's integers
     * \param labels std::string consisting of the labels that should be inserted
     */
    void InsertNewLabels(std::string label);

    /** Comparison Operator == const char label: Returns true if char is in set
     * \param char label
     */
    bool operator ==(const char label) const;
    /** Comparison Operator == string label: Returns true if one of the characters is in the set
     * \param std::string label
     */
    bool operator ==(const std::string label) const;
    /** Comparison Operator == const Label& : Returns true if one of the characters in given set is this set
     * \param std::string label
     */
    bool operator ==(const Label& label) const;

    //friend bool operator <(const Label* lh_label, const Label* rh_label);

    /** Comparison Operator < const char label: Returns true if first label in set is smaller than the rh char
     * \param char label
     */
    bool operator <(const char label);
    /** Comparison Operator == string label: Returns true if first label in set is smaller than the first char in the rh string
     * \param std::string label
     */
    bool operator <(const std::string label);
    /** Comparison Operator == const Label& : Returns true if first char in own set is smaller than first char in rh Label
     * \param std::string label
     */
    bool operator <(const Label& label) const;
    /** Comparison Operator == const Label* : Returns true if first char in own set is smaller than first char in rh Label
     * \param std::string label
     */
    bool operator <(const Label* label);

    /** ostream Operator << const Label& : used for printout of set
     *
     */
    friend std::ostream& operator << (std::ostream& os, const Label& label);

    /** \brief Select first character that matches both labels
     *
     * \param const Label& label
     * \return char: first matching char in both labels
     *
     */
    char select(const Label& label)const;

    /** \brief Rename following rules in map
     *  \param rename_rule: map containing for each
     */
    void rename(std::map<char,std::string>& rename_rules);

    /** Getter Method for labels
    *
    */
    const std::set<char>& GetLabels() const{
        return labels;
    }

    std::string GetLabels_asString() const{
        std::string s;
        for(char c:labels){
            s.push_back(c);
        }
        return s;
    }

    private:
        unsigned int num_labels; //!< Member variable "num_labels"
        std::set<char> labels; //!< Labels
};

#endif /* MODULES_LABEL_H_ */
