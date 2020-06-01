#include <stdio.h>
#include <netinet/in.h>

struct Certs
{
	int cert_id;
    	char ip[128];
    	unsigned int ip_4;
	unsigned int port;
};
/*	规则树节点
**	利用结构体，建立规则树;根节点的父节点为NULL	
*/
struct Certs_node
{
	struct Certs_node *left_node;	//左节点，初始为NULL
	struct Certs_node *right_node;	//右节点，初始为NULL
	struct Certs_node *parent_node;	//父节点，初始为NULL
	int rule_id;
	struct Certs *cert; 
};  

/*  function:   申请树节点
**  parameter:  
**  return:     m_certs_node(Certs_node *) 申请到的树节点指针
*/
struct Certs_node *malloc_certs_node();

/*  function:   销毁树中已有的节点
**  parameter:  c_certs_node(Certs_node *) 要清除树的根节点
				type(int) 1保留根节点，0不保留根节点
				t_pool(struct Pool *) 树节点内存池指针
**  return:     
*/
void clear_certs_tree(struct Certs_node *c_certs_node);

/*  function:   将黑白名单IP插入到树中
**  parameter:  t_tree(Certs_node *) 树根节点
				cert(Certs *) 证书信息
				mask_length(int) 掩码长度
**  return:     
*/
int tree_insert_certs(struct Certs_node *t_tree, struct Certs *cert, int mask_length);

/*  function:	根据IP匹配模型树, 最短匹配
**  parameter:  t_tree(Certs_node *) 规则树节点
				ip(void *) 要匹配的IP
				cert(struct Certs *cert) 返回匹配到的证书信息
**  return:     
*/
int isMatchstr_certs_tree(struct Certs_node *t_tree, void *ip, struct Certs **cert);

int bit_value(void *c_figure, int pos);
