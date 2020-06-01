#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "certs_tree.h"

/*  
 *  function:   申请树节点
 *  parameter:  无
 *  return:     m_certs_node(Certs_node *) 申请到的树节点指针
 */

struct Certs_node *malloc_certs_node()
{
	struct Certs_node *m_certs_node = (struct Certs_node *)malloc(sizeof(struct Certs_node));
	if (m_certs_node == NULL)	return NULL;
	m_certs_node->left_node = NULL;
	m_certs_node->right_node = NULL;
	m_certs_node->parent_node = NULL;
	m_certs_node->rule_id = 0;
	m_certs_node->cert = NULL;

	return m_certs_node;
}

/*  
 *  function:   销毁树中已有的节点
 *  parameter:  c_certs_node(Certs_node *) 要清除树的根节点
 *  return:     
 */
void clear_certs_tree(struct Certs_node *c_certs_node)
{
	if(c_certs_node == NULL)
		return;
	if(c_certs_node->left_node != NULL)
		clear_certs_tree(c_certs_node->left_node);
	if(c_certs_node->right_node != NULL)
		clear_certs_tree(c_certs_node->right_node);
	
	free(c_certs_node->cert);
	free(c_certs_node);
	return;
}

/*  
 *  @function:   将规则业务插入到树中
**  @parameter:  t_tree(Certs_node *) 	树根节点
** 				 cert(struct Certs *) 	证书信息
** 				 mask_length(int) 		掩码长度
** 				 version(int) 			IP地址的版本
**  @return:     
*/
int tree_insert_certs(struct Certs_node *t_tree, struct Certs *cert, int mask_length)
{
	void *p = NULL;
	p = (void *)&(cert->ip_4);
	//插入
	int i;
	for(i = 0; i < mask_length; i++)
	{
		if(bit_value(p, i) == 0)
		{
			if(t_tree->left_node == NULL) {
				t_tree->left_node = malloc(sizeof(struct Certs_node));
				t_tree->left_node->parent_node = t_tree;
			}
			t_tree = t_tree->left_node;
		}
		else
		{

			if(t_tree->right_node == NULL) {
				t_tree->right_node = malloc(sizeof(struct Certs_node));
				t_tree->right_node->parent_node = t_tree;
			}
			t_tree = t_tree->right_node;				
		}
	}
	t_tree->cert = cert;

	return 0;
}

int isMatchstr_certs_tree(struct Certs_node *t_tree, void *ip, struct Certs **cert)
{
	int result = 0;
	if(t_tree == NULL)	return result;

	int tree_depth = 32;
	struct in_addr ip_addr;
	struct in6_addr ip6_addr;	

	int i;
	for(i = 0; i < tree_depth; i++)
	{
		if(bit_value(ip, i) == 0)
		{
			if(t_tree->left_node == NULL)
				break;
			else
			{
				t_tree = t_tree->left_node;
				if(t_tree->cert != NULL)
				{
					*cert = t_tree->cert;
					return 1;					
				
				}
			}
		}
		else
		{
			if(t_tree->right_node == NULL)
				break;
			else
			{
				t_tree = t_tree->right_node;
				if(t_tree->cert != NULL)
				{
					*cert = t_tree->cert;

					return 1;
				}
			}
		}
	}


	return result;
}

int bit_value(void *c_figure, int pos)
{
	uint8_t temp = 1;
        uint8_t figure = 0;

        int num;
        num = pos / 8;
        figure = *((uint8_t *)c_figure + num);

        figure = figure >>(8 -(pos - num * 8) - 1);
        return(figure & temp);
}


int main()
{
	char ip[16] = "192.168.23.3";
	struct sockaddr_in server_addr;
        inet_pton(AF_INET, ip, &server_addr.sin_addr);


	struct Certs_node* root = NULL;
	root = (struct Certs_node* )malloc(sizeof(struct Certs_node));
	if (root == NULL)
		return -1;
	memset(root, 0, sizeof(root));
	struct Certs *cert = NULL;
	cert = (struct Certs *)malloc(sizeof(struct Certs));
	if (cert == NULL)
		return -1;
	memset(cert, 0, sizeof(cert));
	cert->cert_id = 111;
	cert->ip_4 = server_addr.sin_addr.s_addr;
	tree_insert_certs(root, cert, 24);

	struct Certs *tmp;
	if (isMatchstr_certs_tree(root, &server_addr.sin_addr.s_addr, &tmp) == 0)
		printf("查找失败\n");
	else
		printf("查找成功\n");
	printf("tmp.id = %d",tmp->cert_id);

	clear_certs_tree(root);
	return 0;
}
