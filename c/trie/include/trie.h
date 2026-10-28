/**
 * @file trie.h
 * @brief Trie (Prefix Tree) implementation in C11.
 *
 * Implements an alphabet-based prefix tree supporting insertion,
 * exact search, prefix matching, and word deletion.
 */

#ifndef DS_TRIE_H
#define DS_TRIE_H

#include "common.h"
#include <stddef.h>
#include <stdbool.h>

#define TRIE_ALPHABET_SIZE 26

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TrieNode {
    struct TrieNode *children[TRIE_ALPHABET_SIZE]; /**< Child nodes for 'a'..'z' */
    bool is_end_of_word;                           /**< True if node marks end of a word */
} TrieNode;

typedef struct {
    TrieNode *root;     /**< Root node of the Trie */
    size_t word_count;  /**< Total number of distinct words stored */
} Trie;

/**
 * @brief Initializes a Trie with an empty root node.
 */
ds_status_t trie_init(Trie *trie);

/**
 * @brief Destroys all allocated Trie nodes.
 */
void trie_destroy(Trie *trie);

/**
 * @brief Inserts a word (containing only lowercase 'a'-'z') into the Trie.
 */
ds_status_t trie_insert(Trie *trie, const char *word);

/**
 * @brief Checks if exact word is present in the Trie.
 */
bool trie_search(const Trie *trie, const char *word);

/**
 * @brief Checks if any word starts with the specified prefix.
 */
bool trie_starts_with(const Trie *trie, const char *prefix);

/**
 * @brief Removes a word from the Trie and cleans up unused nodes.
 */
ds_status_t trie_remove(Trie *trie, const char *word);

/**
 * @brief Returns total number of words in the Trie.
 */
size_t trie_size(const Trie *trie);

/**
 * @brief Checks if the Trie is empty.
 */
bool trie_is_empty(const Trie *trie);

#ifdef __cplusplus
}
#endif

#endif // DS_TRIE_H
