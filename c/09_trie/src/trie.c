/**
 * @file trie.c
 * @brief Implementation of Trie (Prefix Tree) in C11.
 */

#include "trie.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static TrieNode *trie_node_create(void) {
    TrieNode *node = calloc(1, sizeof(TrieNode));
    return node;
}

static void trie_node_free(TrieNode *node) {
    if (!node) return;
    for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            trie_node_free(node->children[i]);
        }
    }
    free(node);
}

ds_status_t trie_init(Trie *trie) {
    if (!trie) return DS_ERR_NULL_PTR;
    trie->root = trie_node_create();
    if (!trie->root) return DS_ERR_OUT_OF_MEMORY;
    trie->word_count = 0;
    return DS_SUCCESS;
}

void trie_destroy(Trie *trie) {
    if (!trie) return;
    trie_node_free(trie->root);
    trie->root = NULL;
    trie->word_count = 0;
}

ds_status_t trie_insert(Trie *trie, const char *word) {
    if (!trie || !word) return DS_ERR_NULL_PTR;
    if (!trie->root) {
        ds_status_t status = trie_init(trie);
        if (status != DS_SUCCESS) return status;
    }

    TrieNode *curr = trie->root;
    for (size_t i = 0; word[i] != '\0'; i++) {
        unsigned char c = (unsigned char)tolower(word[i]);
        if (c < 'a' || c > 'z') return DS_ERR_INVALID_ARG;

        int idx = c - 'a';
        if (!curr->children[idx]) {
            curr->children[idx] = trie_node_create();
            if (!curr->children[idx]) return DS_ERR_OUT_OF_MEMORY;
        }
        curr = curr->children[idx];
    }

    if (!curr->is_end_of_word) {
        curr->is_end_of_word = true;
        trie->word_count++;
    }
    return DS_SUCCESS;
}

bool trie_search(const Trie *trie, const char *word) {
    if (!trie || !trie->root || !word) return false;

    const TrieNode *curr = trie->root;
    for (size_t i = 0; word[i] != '\0'; i++) {
        unsigned char c = (unsigned char)tolower(word[i]);
        if (c < 'a' || c > 'z') return false;

        int idx = c - 'a';
        if (!curr->children[idx]) return false;
        curr = curr->children[idx];
    }
    return curr->is_end_of_word;
}

bool trie_starts_with(const Trie *trie, const char *prefix) {
    if (!trie || !trie->root || !prefix) return false;

    const TrieNode *curr = trie->root;
    for (size_t i = 0; prefix[i] != '\0'; i++) {
        unsigned char c = (unsigned char)tolower(prefix[i]);
        if (c < 'a' || c > 'z') return false;

        int idx = c - 'a';
        if (!curr->children[idx]) return false;
        curr = curr->children[idx];
    }
    return true;
}

static bool trie_has_children(const TrieNode *node) {
    for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
        if (node->children[i]) return true;
    }
    return false;
}

static bool trie_remove_helper(TrieNode *node, const char *word, size_t depth, bool *removed) {
    if (!node) return false;

    if (depth == strlen(word)) {
        if (node->is_end_of_word) {
            node->is_end_of_word = false;
            *removed = true;
        }
        // If node has no children, it can be deleted
        return !trie_has_children(node);
    }

    unsigned char c = (unsigned char)tolower(word[depth]);
    if (c < 'a' || c > 'z') return false;
    int idx = c - 'a';

    if (trie_remove_helper(node->children[idx], word, depth + 1, removed)) {
        free(node->children[idx]);
        node->children[idx] = NULL;
        return !node->is_end_of_word && !trie_has_children(node);
    }
    return false;
}

ds_status_t trie_remove(Trie *trie, const char *word) {
    if (!trie || !trie->root || !word) return DS_ERR_NULL_PTR;

    bool removed = false;
    trie_remove_helper(trie->root, word, 0, &removed);
    if (removed) {
        trie->word_count--;
        return DS_SUCCESS;
    }
    return DS_ERR_NOT_FOUND;
}

size_t trie_size(const Trie *trie) {
    return trie ? trie->word_count : 0;
}

bool trie_is_empty(const Trie *trie) {
    return !trie || trie->word_count == 0;
}

