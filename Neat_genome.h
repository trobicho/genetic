/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Neat_genome.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/03 00:26:11 by trobicho          #+#    #+#             */
/*   Updated: 2019/08/04 13:59:12 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef struct	s_node_gene
{
	int		layer;
	double	in;
	int		nb_until_finish;
}				t_node_gene;

typedef struct	s_connection_gene
{
	int		node_in;
	int		node_out;
	double	weight;
	bool	enabled;
	int		innov;
	bool	done;
}				t_connection_gene;

using namespace std;

class	Neat_genome
{
	public:
		Neat_genome(int nb_input, int nb_output);
        
		template <typename T>
        vector<double>&	calc(const std::vector<T> &entrie)
        {
            double	r;
			bool	finish = false;
            for(int n=0; n<m_node_gene->size(); n++)
            {
				if (m_node_gene[i].layer == 0)
				{
					m_node_gene[n].out = (double)entrie[n];
					m_node_gene[n].calc_finish = true;
				}
				else
				{
					m_node_gene[n].in = 0.0;
					m_node_gene[n].nb_until_finish = 0;
				}
            }
			for(int c=0; c < m_connec_gene->size(); c++)
			{
				m_connec_gene[c].done = false;
				m_node_gene[m_connec_gene[c].out].nb_until_finish++;
			}
			while (finish == false)
			{
				finish = true;
				for(int c=0; c < m_connec_gene->size(); c++)
				{
					if (m_connec_gene[c].done == false && m_connec_gene[c].enabled)
					{
						int	in = m_connec_gene[c].in;
						int	out = m_connec_gene[c].out;
						if (m_node_gene[in].nb_until_finish <= 0)
						{
							m_node_gene[out].in += m_node_gene[in].in * m_connec_gene[c].weight;
							m_node_gene[out].nb_until_finish--;
						}
						else
							finish = false;
					}
				}
			}
			int	r = 0;
            for(int n=0; n<m_node_gene->size(); n++)
			{
				if (m_node_gene[n].layer == 2)
					m_result[r++] = m_node_gene[n].in;
			}
            return m_result;
        }

	private:
		vector<t_node_gene>			m_node_gene;
		vector<t_connection_gene>	m_connec_gene;
		vector<double>				m_result;
}
