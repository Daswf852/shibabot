#pragma once

/* Explanation:
 * 
 * A source tree will contain all possible permissions a tree can have
 * New permissions will be insertable to the tree but not removed
 * 
 * Permission trees will be a subset of the source tree
 * 
 * Example permissions:
 * foo.bar.baz
 * foo.bar
 * foo.qux
 * 
 */

#include <memory>
#include <optional>
#include <regex>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

namespace Shiba::Perm {

struct PermissionException : public std::exception {};
//Thrown when a bad identifier is contained within a SinglePermisson
struct BadIdentifier : public PermissionException {};

struct TreeException : public std::exception {};
//Thrown when a tree tries to operate on an empty permission
struct EmptyPermission : public TreeException {};
//Thrown when a tree tries to operate on a missing identifier
//For example when trying to GetChild with an identifier that isn't contained in that node's children
struct IdentifierNotFound : public TreeException {};
//Thrown when a given permission to operate on could not be found
//For example when trying to remove a SinglePermission whic his not Contain-ed
struct PermissionNotFound : public TreeException {};
//Thrown when a tree tries to access its parent when it is std::nullopt
struct NoParent : public TreeException {};
//Thrown when a tree tries to operate on its children when they don't exist
//For example when trying to GetChild with empty this->children
struct NoChildren : public TreeException {};
//Thrown when the root identifier of a SinglePermission doesn't match that of a root node's identifier
struct BadRoot : public TreeException {};
//struct NotLeaf : public TreeException {};


typedef std::vector<std::string> SinglePermission;
SinglePermission MakeSinglePermission(const std::string &permission);
std::ostream &operator<<(std::ostream &stream, const Shiba::Perm::SinglePermission &spermission);

//some notes:
//the node with no parent is always identified with "root"
class TreeNode {
    public:
        TreeNode(TreeNode &&node) 
        : identifier(node.identifier)
        , parentNode(node.parentNode)
        , children(std::move(node.children)) {};

        TreeNode(const SinglePermission &permission);
        TreeNode(std::optional<std::reference_wrapper<TreeNode>> parent, std::string identifier);

        const std::string &GetIdentifier() const { return identifier; }

        //Returns the single permission of a node while disregarding the children
        SinglePermission GetSinglePermission() const;

        TreeNode &Insert(const std::string &identifier);
        TreeNode &Insert(const SinglePermission &permission);

        void Remove(const std::string &identifier);
        void Remove(const SinglePermission &permission);

        //Returns a reference to the child with the given identifier
        //Throws Shiba::Perm::IdentifierNotFound if the child cannot be found
        TreeNode &GetChild(const std::string &identifier);

        //Returns a reference to the child with the given identifier
        //Throws Shiba::Perm::IdentifierNotFound if the child cannot be found
        const TreeNode &GetChild(const std::string &identifier) const;

        //Returns a reference to the child with the given SinglePermission
        TreeNode &GetChild(const SinglePermission &permission);

        //Returns a reference to the child with the given SinglePermission
        const TreeNode &GetChild(const SinglePermission &permission) const;

        //Returns a reference to the parent of the node
        //Throws Shiba::Perm::NoParent if the node has no parent (aka if it is a root node)
        TreeNode &GetParent();

        //Returns a reference to the parent of the node
        //Throws Shiba::Perm::NoParent if the node has no parent (aka if it is a root node)
        const TreeNode &GetParent() const;

        //Writes the children in an indented tree format to a given stream
        void GetTree(std::ostream &stream, int indent = 0, bool printRoot = false) const;

        //checks if the children contains a node with the given identifier
        //doesn't throw unlike GetChild
        bool Contains(const std::string &identifier) const;

        //checks for example if a permission bot.coreModule.echo permission exists in a user's permission tree
        //bool permissionGranted = userTree.Contains(perm)
        //if a tree has foo.bar.baz, it contains foo.bar
        //if a tree has foo.bar it doesn't contain foo.bar.baz
        bool Contains(const SinglePermission &perm) const;

        //checks for example if a user's permission tree is valid
        //bool valid = sourceTree.Contains(userTree)
        ///TODO: fix this lazy implementation with a faster one...
        bool Contains(const TreeNode &node) const;

        friend std::ostream &operator<<(std::ostream &stream, const Shiba::Perm::TreeNode &node);
        
    private:
        std::string identifier;
        std::optional<std::reference_wrapper<TreeNode>> parentNode;
        std::vector<std::unique_ptr<TreeNode>> children;
};



}