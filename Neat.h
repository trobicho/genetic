/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Neat.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/03 00:18:12 by trobicho          #+#    #+#             */
/*   Updated: 2019/08/04 17:32:19 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Neat: public AbsGenetic
{
    public:
        Neat(){};
        Neat(Fitness *fitness, int nbPeople):Neat(fitness, nbPeople, nbPeople*3){};
        Neat(Fitness *fitness, int nbPeople, int peopleLen);
        ~Neat();

        int				get_bestScore() {return (m_people[0]->get_score();)}
        Neat_genome&	get_bestGene() {return (m_people[0]->get_gene();)}
        void			nextGen();
        int				crossing();
        int				mutate();
        void			parentSelect();
        void			elitistSort();
		int				get_innovation_number(void) {return (m_innovation_number);}
		int				get_new_innovation_number(void) {return (m_innovation_number++);}


    protected:
        std::vector<Neat_people*>	m_people;
        Fitness						*m_fitness;
        int 						m_nb_stable_people;
        int 						m_nbCrossing;
		double						m_mutate_prob;
		int							m_innovation_number = 0;

    	Neat_genome					m_genome_copy;

    private:
        std::vector<Neat_people*>	m_parent;
};
