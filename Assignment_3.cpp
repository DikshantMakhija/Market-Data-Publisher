#include <iostream>
using namespace std;

struct Node{        //Node for operations
    long int id,price;
    Node *left,*right;
    int height;
};

struct Node1{       //Node for AVL formation
    long int id,price;
};

Node *root;

long int max(long int a,long int b){        //Max function
    return (a > b)? a : b;
}

Node *Nod(long int id, long int price){     //New Node
    Node *x;
    x = new Node();
    x->id = id;
    x->price = price;
    x->right = NULL;
    x->left = NULL;
    return x;
}

long int diff(Node *node){                  //diff between left and right height
    if(!node)       return 0;
    if(node->left && node->right){
        return node->left->height - node->right->height;
    }
    else if(node->left){
        return node->left->height;
    }
    else if(node->right){
        return node->right->height;
    }
    return 0;
}

void merge(Node1 arr[], long int l, long int m,long int r){ //merge function
	long int i,j,k;
	long int x = m-l+1;
	long int y = r-m;

    Node1 A[x],B[y];

    for (i=0;i<x;i++){
        A[i].id = arr[l+i].id;
        A[i].price = arr[l+i].price;
    }
	for (j=0;j<y;j++){
        B[j].id = arr[m+1+j].id;
        B[j].price = arr[m+1+j].price;
    }
	i = 0; j = 0;  k = l;
	while(i<x && j<y){                     //putting values in array from A
		if (A[i].id<=B[j].id){
			arr[k].id = A[i].id;
            arr[k].price = A[i].price;
			i++;
		}
		else{                             //putting values in array from B
			arr[k].id = B[j].id;
            arr[k].price = B[j].price;
			j++;
		}
		k++;
	}
	while(i<x){                            //putting remaining values(if any)
		arr[k].id = A[i].id;
        arr[k].price = A[i].price;
		i++;
		k++;
	}
	while(j<y){                            //putting remaining values(if any)
		arr[k].id = B[j].id;
        arr[k].price = B[j].price;
		j++;
		k++;
	}
}

void mergeSort(Node1 arr[],long int l,long int r){      //merge sort function
	if(l>=r)       return;
    if (l < r)
	{
		long int m = l+(r-l)/2;
		mergeSort(arr, l, m);
		mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
	}
}

int height(Node *tra){
    if(!tra)      return 0;
    else{
        return max(height(tra->left),height(tra->right)) + 1;
    }
}

Node* form_avl(Node1 cmpny[],long int i,long int j){
    if(i>j)        return NULL;
    Node *node = Nod(cmpny[(i+j)/2].id,cmpny[(i+j)/2].price);       //creating node from company id and price

    node->left = form_avl(cmpny,i,((i+j)/2)-1);
    node->right = form_avl(cmpny,((i+j)/2)+1,j);

    if(i==(i+j)/2)                                           //taking heights (for i==j or i = j-1 this condition holds)
        node->height = max(height(node->left),height(node->right))+1;
    else
        node->height = max((node->left->height),(node->right->height))+1;

    return node;
}

void print(Node *node){                         //printing nodes in preOrder traversal
    if (!node)        return;
    cout<<node->id<<" "<<node->price<<" "<<node->height<<"\n";
    print(node->left);
    print(node->right);
}

Node* traverse(Node *tr, long int id){          //traversing node for insert
    if(tr){
        if(tr->id==id)                return tr;
        if(tr->id<id){
            if(tr->right)             return traverse(tr->right,id);
            else return tr;
        }
        else{
            if(tr->left)              return traverse(tr->left,id);
            else return tr;
        }
    }
    return NULL;
}


Node* traverseup(Node *tr, long int id){        //traversing node for update or stock_split etc
    if(tr){
        if(tr->id==id)                return tr;
        if(tr->id<id){
            if(tr->right)             return traverseup(tr->right,id);
            //else return tr;
        }
        else{
            if(tr->left)              return traverseup(tr->left,id);
            //else return tr;
        }
    }
    return NULL;
}

Node *rotate_right(Node *a){                    //right rotation
    Node *y = a->left;
    Node *y2 = y->right;
    a->left = y2;
    y->right = a;

    if(a->right && a->left)         a->height = max(a->left->height, a->right->height)+1;
    else                            a->height = height(a->left) + 1;
    if(y->left && y->right)         y->height = max(y->left->height, y->right->height)+1;
    else                            y->height = height(y->right) + 1;

    return y;
}

Node *rotate_left(Node* b){                     //left rotation
    Node *y = b->right;
    Node *y2 = y->left;
    b->right = y2;
    y->left = b;

    if(b->right && b->left)         b->height = max(b->left->height, b->right->height)+1;
    else                            b->height = height(b->left) + 1;
    if(y->left && y->right)         y->height = max(y->left->height, y->right->height)+1;
    else                            y->height = height(y->right) + 1;

    return y;
}

int balance_height(Node *node,long int id){     //balancing height from root to node with id
    if(node){
        if(node->left && node->id>id)           node->left->height = balance_height(node->left,id);
        if(node->right && node->id<id)          node->right->height = balance_height(node->right,id);

        if(node->left && node->right)           return max(node->left->height,node->right->height)+1;
        else                                    return max(height(node->left),height(node->right))+1;
    }
    return 0;
}

Node *rearrange(Node *node){                    //rearrange function for one particular node
    Node *y;
    int a=0;
    if(!node)           return NULL;
    if(height(node->left)>height(node->right)+1){
        a = 1;
    }
    if(height(node->left)<height(node->right)-1){
        a = 2;
    }
    switch(a){
        case 1:
            y = node->left;
            if(y){
                if(height(y->left)>height(y->right)){
                    return rotate_right(node);
                }
                if(height(y->left)<height(y->right)){
                    node->left = rotate_left(y);
                    return rotate_right(node);
                }
            }
            break;
        case 2:
            y = node->right;
            if(y){
                if(height(y->left)>height(y->right)){
                    node->right = rotate_right(y);
                    return rotate_left(node);
                }
                if(height(y->left)<height(y->right)){
                    return rotate_left(node);
                }
            }
            break;
    }
    return node;
}

Node* balance(Node *node, long int id){         //balancing from root to node with id

    if(node->left && node->id > id)              node->left = balance(node->left,id);
    else if(node->right && node->id < id)        node->right = balance(node->right,id);

    if(diff(node)>1 || diff(node)<-1)               return rearrange(node);

    else                return node;
}

void insert(Node *node, long int id, long int price){       //insertion function
    Node *temp = new Node();
    Node *temp1 = new Node();

    temp=traverse(root,id);                                 //traversing to node's parent for insertion

    if(!root){                                              //if root=NULL
        root=Nod(id,price);
        cout<<id<<" "<<price<<"\n";
    }
    else if(temp->id>id){                                   //if node's id is >id
        temp->left=Nod(id,price);
        cout<<id<<" "<<price<<"\n";
    }
    else if(temp->id<id){                                   //if node's id is <id
        temp->right=Nod(id,price);
        cout<<id<<" "<<price<<"\n";
    }
}

Node *traverse_parent(Node *trp,Node *node){                //traversing parent for node
    if(node==root || !node || !trp)          return NULL;
    else if(trp->right && trp->id<node->id){
            if(trp->right==node)            return trp;
            else                            return traverse_parent(trp->right,node);
        }
    else if(trp->left && trp->id>node->id){
            if(trp->left==node)             return trp;
            else                            return traverse_parent(trp->left,node);
        }
    return NULL;
}

void update(Node *root,long int id,long int price, long int threshold){     //updating the price at id
    Node *node = traverseup(root,id);
    if(!node)       return;                                                 //in case node doesn't exist
    if(node->id == id){
        if(price-node->price>threshold || price-node->price<(-1)*threshold){    //rule for updating
            node->price = price;
            cout<<node->id<<" "<<node->price<<"\n";
        }
    }
}

Node *find_min(Node *node){                                             //finding successor for the node
    if(node->left)      return find_min(node->left);
    else                return node;
}

Node *deregister(Node *node, long int id){                              //function for deregister
  if(node){
    if(node->left && node->right){                                      //if node has 2 children
        Node *min = find_min(node->right);
        Node *min_parent = traverse_parent(root,min);
        node->id = min->id;
        node->price = min->price;
        if(min_parent->left==min){                                      //if successor is left child
                min_parent->left = deregister(min,min->id);
            }
        else if(min_parent->right==min){                                //if successor is right child
                min_parent->right = deregister(min,min->id);
            }
        return node;
    }
    else if(node->right){                                               //if node has single child i.e, right
            return node->right;
        }
    else if(node->left){                                                //if node has single child i.e, left
            return node->left;
        }
    else{                                                               //if node has no child
            return NULL;
        }
    }
    return NULL;
}

void stockSplit(Node *root,long int id,long int x,long int y){
    Node *node = traverseup(root,id);                                   //traversing to the node
    if(!node)       return;                                             //if node doesn't exist
    if(node->id == id){                                                 //if node's found
        long int temp = node->price;
        node->price = (temp*y)/x;
        cout<<node->id<<" "<<node->price<<"\n";
    }
}

int main() {
    long int n;
    cin>>n;
    //Node *root;
    Node *temp;
    if(n>0){
        Node1 cmpny[n];
        for(long int i=0;i<n;i++)    cin>>cmpny[i].id>>cmpny[i].price;
        mergeSort(cmpny,0,n-1);
        root = form_avl(cmpny,0,n-1);
    }
    if(n==0)        root = NULL;                    //if the input array doesn't exist i.e, no of elements in array are 0
    long int q,t;
    cin>>q>>t;                                      //input of no of operations and threshold
    for(long int i=0;i<q;i++){                      //loop for multiple operations
        long int x;
        cin>>x;
        long int a,b;
        switch(x){
            case 1:
                cin>>a>>b;
                insert(root,a,b);                   //insertion
                root = balance(root,a);             //balancing unbalanced nodes till root
                root->height = balance_height(root,a);  //balancing height
                break;
            case 2:
                cin>>a;
                temp = traverseup(root,a);          //traversing for deletion
                if(temp){                           //if node is found(or exists)
                    Node *par = traverse_parent(root,temp);         //traversing parent
                    if(par){                                        //if parent exists
                        int z=0;
                        if(par->left==temp){                        //if left child of parent is node
                            par->left = deregister(temp,a);
                            z=1;
                        }
                        else if(par->right==temp){                  //if right child of parent is node
                            par->right = deregister(temp,a);
                            z=1;
                        }
                        if(z==1){                   //if deregistered a node balancing height and rearranging till root
                            root = balance(root,par->id);
                            root->height = balance_height(root,par->id);
                        }
                    }
                    else if(temp==root){            //if the node to be deleted is root itself
                        root = deregister(temp,a);
                    }
                }
                break;
            case 3:
                cin>>a>>b;
                update(root,a,b,t);                 //updating the price
                break;
            case 4:
                char c;
                long int r[2];
                cin>>a>>r[0]>>c>>r[1];              //input int char int for e.g., 2:1 (char is colon)
                stockSplit(root,a,r[0],r[1]);       //stockSplit case for given node
                break;
        }
    }
    return 0;
}
