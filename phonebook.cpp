#include <iostream>
#include <string>

struct Contact
{
    std::string name;
    std::string phoneNumber;
    Contact(std::string n, std::string num) : name(n), phoneNumber(num) {}
};

struct Node
{
    Contact contact;
    Node *left;
    Node *right;
    int height;
    Node(Contact c) : contact(c), left(nullptr), right(nullptr), height(1) {}
};

class AVLPhonebook
{
private:
    Node *root;

    int height(Node *node)
    {
        return (node == nullptr) ? 0 : node->height;
    }

    int balanceFactor(Node *node)
    {
        return (node == nullptr) ? 0 : height(node->left) - height(node->right);
    }

    Node *rotateRight(Node *y)
    {
        Node *x = y->left;
        Node *T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Node *rotateLeft(Node *x)
    {
        Node *y = x->right;
        Node *T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;

        return y;
    }

    Node *balance(Node *node)
    {
        if (node == nullptr)
            return node;

        node->height = std::max(height(node->left), height(node->right)) + 1;

        int bf = balanceFactor(node);

        if (bf > 1)
        {
            if (balanceFactor(node->left) >= 0)
                return rotateRight(node);
            else
            {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }

        if (bf < -1)
        {
            if (balanceFactor(node->right) <= 0)
                return rotateLeft(node);
            else
            {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
    }

    Node *insert(Node *node, Contact contact)
    {
        if (node == nullptr)
            return new Node(contact);

        if (contact.name < node->contact.name)
            node->left = insert(node->left, contact);
        else if (contact.name > node->contact.name)
            node->right = insert(node->right, contact);
        else
            return node; // Duplicate contacts not allowed

        return balance(node);
    }

    Node *findMin(Node *node)
    {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    Node *remove(Node *node, std::string name)
    {
        if (node == nullptr)
            return node;

        if (name < node->contact.name)
            node->left = remove(node->left, name);
        else if (name > node->contact.name)
            node->right = remove(node->right, name);
        else
        {
            if (node->left == nullptr || node->right == nullptr)
            {
                Node *temp = (node->left) ? node->left : node->right;
                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                {
                    *node = *temp;
                }
                delete temp;
            }
            else
            {
                Node *temp = findMin(node->right);
                node->contact = temp->contact;
                node->right = remove(node->right, temp->contact.name);
            }
        }

        if (node == nullptr)
            return node;

        return balance(node);
    }

    Node *search(Node *node, std::string name)
    {
        if (node == nullptr || node->contact.name == name)
            return node;

        if (name < node->contact.name)
            return search(node->left, name);
        else
            return search(node->right, name);
    }

public:
    AVLPhonebook() : root(nullptr) {}

    void addContact(std::string name, std::string phoneNumber)
    {
        root = insert(root, Contact(name, phoneNumber));
    }

    void removeContact(std::string name)
    {
        root = remove(root, name);
    }

    Contact *searchContact(std::string name)
    {
        Node *node = search(root, name);
        if (node)
            return &(node->contact);
        else
            return nullptr;
    }
};

int main()
{
    AVLPhonebook phonebook;

    phonebook.addContact("John Doe", "123-456-7890");
    phonebook.addContact("Jane Smith", "987-654-3210");
    phonebook.addContact("Alice Johnson", "555-555-5555");

    Contact *contact = phonebook.searchContact("Jane Smith");
    if (contact)
    {
        std::cout << "Contact found: " << contact->name << " - " << contact->phoneNumber << std::endl;
    }
    else
    {
        std::cout << "Contact not found." << std::endl;
    }

    phonebook.removeContact("Jane Smith");

    contact = phonebook.searchContact("Jane Smith");
    if (contact)
    {
        std::cout << "Contact found: " << contact->name << " - " << contact->phoneNumber << std::endl;
    }
    else
    {
        std::cout << "Contact not found." << std::endl;
    }

    return 0;
}