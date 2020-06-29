#include "perm.hpp"

namespace Shiba::Perm {

SinglePermission MakeSinglePermission(const std::string &permission) {
    SinglePermission spermission;

    std::stringstream ss(permission);
    std::string current;

    std::regex permissionRegex("[a-zA-Z]*");

    while (std::getline(ss, current, '.')) {
        if (!std::regex_match(current, permissionRegex)) {
            throw BadIdentifier();
        }
        spermission.push_back(current);
    }

    return spermission;
}

std::ostream &operator<<(std::ostream &stream, const Shiba::Perm::SinglePermission &spermission) {
    bool first = true;
    for (const std::string &perm : spermission) {
        if (!first) stream<<'.';
        first = false;
        stream<<perm;
    }
    return stream;
}


TreeNode::TreeNode(const SinglePermission &permission) : identifier("root") {
    //if (permission.empty()) throw EmptyPermission();

    std::reference_wrapper<TreeNode> treeRef = *this;
    for (const std::string &ident : permission) {
        treeRef = treeRef.get().Insert(ident);
    }
}

TreeNode::TreeNode(std::optional<std::reference_wrapper<TreeNode>> parent, std::string identifier)
: identifier((parent == std::nullopt)?"root":identifier)
, parentNode(parent) {}

TreeNode &TreeNode::Insert(const std::string &identifier) {
    if (Contains(identifier)) return GetChild(identifier);
    
    children.push_back(std::make_unique<TreeNode>(*this, identifier));
    return *children.back();
}

TreeNode &TreeNode::Insert(const SinglePermission &permission) {
    if (permission.empty()) throw EmptyPermission();
    //if (permission.size() == 1) if (permission.at(0) == identifier) throw BadRoot();
    //that was retareded...

    std::reference_wrapper<TreeNode> currentNode = *this;
    for (const std::string &ident : permission) {
        try {
            currentNode = currentNode.get().GetChild(ident);
        } catch (TreeException &e) {
            currentNode = currentNode.get().Insert(ident);
        }
    }

    return currentNode;
}

void TreeNode::Remove(const std::string &identifier) {
    std::vector<std::vector<std::unique_ptr<Shiba::Perm::TreeNode>>::iterator> toRemove;
    
    for (std::vector<std::unique_ptr<Shiba::Perm::TreeNode>>::iterator it = children.begin(); it != children.end(); it++) {
        if ((*it)->GetIdentifier() == identifier) toRemove.push_back(it);
    }

    if (toRemove.empty()) throw IdentifierNotFound();

    for (auto it : toRemove) children.erase(it);
}

void TreeNode::Remove(const SinglePermission &permission) {
    if (permission.empty()) throw EmptyPermission();
    if (!Contains(permission)) throw PermissionNotFound();
    
    try {
        TreeNode &target = GetChild(permission);
        TreeNode &parent = target.GetParent();
        parent.Remove(target.GetIdentifier());
    } catch (TreeException &e) {
        throw e;
    }
}

//Returns a reference to the child with the given identifier
//Throws Shiba::Perm::IdentifierNotFound if the child cannot be found
TreeNode &TreeNode::GetChild(const std::string &identifier) {
    try {
        return const_cast<TreeNode &>(std::as_const(*this).GetChild(identifier));
    } catch (std::exception &e) { throw e; }
    
}

//Returns a reference to the child with the given identifier
//Throws Shiba::Perm::IdentifierNotFound if the child cannot be found
const TreeNode &TreeNode::GetChild(const std::string &identifier) const {
    if (children.empty()) throw NoChildren();
    for (const std::unique_ptr<TreeNode> &uptr : children) {
        if (uptr->GetIdentifier() == identifier) return *uptr;
    }
    throw IdentifierNotFound();
}

TreeNode &TreeNode::GetChild(const SinglePermission &permission) {
    try {
        return const_cast<TreeNode &>(std::as_const(*this).GetChild(permission));
    } catch (std::exception &e) { throw e; }
    
}

const TreeNode &TreeNode::GetChild(const SinglePermission &permission) const {
    if (permission.empty()) throw EmptyPermission();

    std::reference_wrapper<const TreeNode> ref = *this;
    for (const std::string &identifier : permission) {
        try {
            ref = ref.get().GetChild(identifier);
        } catch (TreeException &e) {
            throw e;
        }
    }

    return ref.get();
}

//Returns a reference to the parent of the node
//Throws Shiba::Perm::NoParent if the node has no parent (aka if it is a root node)
TreeNode &TreeNode::GetParent() {
    try {
        return const_cast<TreeNode &>(std::as_const(*this).GetParent());
    } catch (std::exception &e) { throw e; }
}

//Returns a reference to the parent of the node
//Throws Shiba::Perm::NoParent if the node has no parent (aka if it is a root node)
const TreeNode &TreeNode::GetParent() const {
    if (parentNode.has_value()) return parentNode.value();
    throw NoParent();
}

//Returns the single permission of a node while disregarding the children
SinglePermission TreeNode::GetSinglePermission() const {
    //if (children.size()) throw NotLeaf();
    SinglePermission reversePerms;

    std::reference_wrapper<const Shiba::Perm::TreeNode> storedNode = *this;

    bool done = false;
    while (!done) {
        reversePerms.push_back(storedNode.get().GetIdentifier());
        try {
            storedNode = storedNode.get().GetParent();
        } catch (...) { done = true; }
    }

    std::reverse(reversePerms.begin(), reversePerms.end());
    return reversePerms;
}

//Writes the children in an indented tree format to a given stream
void TreeNode::GetTree(std::ostream &stream, int indent, bool printRoot) const {
    for (int i = 0; i < indent; i++) stream<<' ';
    if (!indent && !printRoot) stream<<std::endl;
    if (indent || printRoot) stream<<identifier<<std::endl;
    for (const std::unique_ptr<TreeNode> &ptr : children) {
        ptr->GetTree(stream, indent+1);
    }
}

//checks if the children contains a node with the given identifier
//doesn't throw unlike GetChild
bool TreeNode::Contains(const std::string &identifier) const {
    if (children.empty()) return false;
    for (const std::unique_ptr<TreeNode> &node : children) {
        if (node->GetIdentifier() == identifier) return true;
    }
    return false;
}

//checks for example if a permission bot.coreModule.echo permission exists in a user's permission tree
//bool permissionGranted = userTree.Contains(perm)
//if a tree has foo.bar.baz, it contains foo.bar
//if a tree has foo.bar it doesn't contain foo.bar.baz
bool TreeNode::Contains(const SinglePermission &perm) const {
    if (perm.empty()) throw EmptyPermission();
    
    std::reference_wrapper<const TreeNode> currentNode = *this;
    for (const std::string &ident : perm) {
        try {
            currentNode = currentNode.get().GetChild(ident);
        } catch (NoChildren) {
            return true;
        } catch (IdentifierNotFound) {
            return false;
        }
    }
    return true;
}

//checks for example if a user's permission tree is valid
//bool valid = sourceTree.Contains(userTree)
///TODO: fix this lazy implementation with a faster one...
bool TreeNode::Contains(const TreeNode &node) const {
    if (node.children.empty()) return this->Contains(node.GetSinglePermission());
    
    bool booleanSum = true;
    for (const std::unique_ptr<TreeNode> &child : children) booleanSum |= this->Contains(*child);
    
    return booleanSum;
}

std::ostream &operator<<(std::ostream &stream, const Shiba::Perm::TreeNode &node) {
    //First insert the parents
    stream<<node.GetSinglePermission();

    //Then insert the children tree
    node.GetTree(stream, 0);

    return stream;
}

}