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

#ifndef MODULES_RULE_H_
#define MODULES_RULE_H_

#include<vector>
#include<iostream>
#include<set>
#include"Label.h"

//forward declarations
class Rule;
typedef std::pair<Rule,std::vector<int>> ruleset;

class Rule
{
    public:
        /** Default constructor */
        Rule();

        Rule(std::string rule_string);
        /** Default destructor */
        virtual ~Rule();
        /** \brief add new label to rule
         *
         * \param std::string label
         *
         */
        void emplace_back(std::string label){
            rule.emplace_back(label);
            size++;
        }
        void push_back(Label label){
            rule.push_back(label);
            size++;
        }
        /** \brief find
         *
         * \param std::string label label that should be found
         * \param std::set<int>& out reference to set that will contain the indexes of all rule positions that match the
         */
        void Get_All_Permutation(std::vector<ruleset>& out);

        void print(){
            for(Label elem:rule){
                std::cout << elem <<" ";
            }
            std::cout << std::endl;
        }

        /** Comparison Operator == const Rule&: Returns true if the labels match pairwise by position.
         * \param std::string label
         */
        bool operator ==(const Rule& rh_rule) const;

        /** \brief Get i-th label stored in rule
         *
         * \param unsigned int i: Index of required label set
         * \return const Label&: returns a reference (for efficiency) to label-set at i-th position. Note const!
         */
        const Label& operator[](unsigned int i) const;

        /** \brief Set i_th element rule
         *
         *  \param unsigned int i: position where label should be set
         *  \param const Lable&: new Label
         *  \return bool: true if setting was successful
         */
        bool setLabel(unsigned int i, const Label& newLabel){
            if(i<size){
                rule[i] = newLabel;
            }
            else{
                return false;
            }
            return true;
        }

        /** \brief Set i_th element rule
         *
         *  \param unsigned int i: position where label should be set
         *  \param std::string: new Label
         *  \return bool: true if setting was successful
         */
        bool setLabel(unsigned int i, const std::string newLabel){
            if(i<size){
                rule[i] = Label(newLabel);
            }
            else{
                return false;
            }
            return true;
        }

        /** \brief Check if at least one permutation of this rule matches the given rule. If it finds one it stores the result in found_perm
         *
         * \param const Rule& rule_to_check: rule that should be compared to this rule
         * \param ruleset& found_perm: first matching permutation that matches  rule_to_check
         * \return bool: true if a matching permutation is found
         */
        bool check_all_permutations(const Rule& rule_to_check, ruleset& found_perm) const;
         /** \brief Check if at least one permutation of this rule matches on of the given rules. If it finds one it stores the result in found_perm
         *
         * \param const std::vector<Rule>& rules_to_check: rules that should be compared to this rule
         * \param ruleset& found_perm: first matching permutation that matches  rule_to_check
         * \return int: index of matching rule or -1 if not found
         */
        int check_all_permutations_set(const std::vector<Rule>& rules_to_check, ruleset& found_perm) const;

        /** \brief Select subset of this rule such that it fits the given rule.
         *
         *  ! Changes object
         *
         * \param const Rule&: rule_to_check
         * \param std::vector<int> order
         * \return std::vector<char> vector containing the selected labels
         *
         */
         void select_following_Rule(const Rule& selection_rule, std::vector<int> order = {});

         /** \brief Rename following rules in map
          *  \param rename_rule: map containing for each
          */
         void rename(std::map<char,std::string>& rename_rule){
             for(int i=0;i<size;i++){
                 rule[i].rename(rename_rule);
             }
         }

         friend std::ostream& operator<<(std::ostream& os, const Rule& rule);

    protected:

    private:
        //void permute(std::vector<std::pair<Label;std::set<int>>& out, std::vector<Label> rule);
        int size;
        std::vector<Label> rule;
};

std::ostream& operator<<(std::ostream& os, const Rule& rule);
std::ostream& operator<<(std::ostream& os, const ruleset& ruleset);


#endif /* MODULES_RULE_H_ */
