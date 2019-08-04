/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Neat_genome.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/03 00:29:13 by trobicho          #+#    #+#             */
/*   Updated: 2019/08/04 20:06:44 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Neat_genome.h"
#include "random.h"

Neat_genome(int nb_input, int nb_output)
{
}

void	mutate_add_node(void)
{
	int r;

	while (int i = 0; i < 10; i++)
	{
		r = randMinMax(0, m_connec_gene->size);
		if (m_connec_gene[r].enabled)
			break;
	}
	if (m_connec_gene[r].enabled)
	{
		t_node	node;
		/*
		node.layer = m_node_gene[m_connec_gene[r].node_in].layer + 1;
		if (m_node_gene[m_connec_gene[r].node_out].layer
			> m_node_gene[m_connec_gene[r].node_in].layer)
			node.layer = m_node_gene[m_connec_gene[r].node_out].layer - 1;
		*/
		node.layer = 1;
		m_node_gene.push_back();
		m_connec_gene[r].enabled = false;
		m_connec_gene.push_back();
		m_connec_gene.back() = m_connec_gene[r];
		m_connec_gene.back().out = m_node_gene.size() - 1;
		m_connec_gene.back().innov = get_new_innovation_number();
	}
}
