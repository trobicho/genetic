/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Neat_people.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/03 00:20:06 by trobicho          #+#    #+#             */
/*   Updated: 2019/08/04 21:58:26 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AbsPeople.h"

class	Neat_people: public AbsPeople<double>
{
	public:
        Neat_people():AbsPeople(){};
        Neat_people(Fitness *fitness, int nbGene, int generation=1): AbsPeople(nbGene, generation), m_fitness(fitness){};
        int evaluate(int generation){return (m_score=m_fitness->evaluate(m_gene, generation));}
        int evaluate(){return (m_score=m_fitness->evaluate(m_gene));}
}
