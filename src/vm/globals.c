#include <stdlib.h>
#include <string.h>
#include "../../include/algo_vm.h"
#include "../../include/algo_value.h"

typedef struct {
    ObjString* key;
    Value value;
    bool exists;
} GlobalEntry;

static GlobalEntry* global_table = NULL;
static int global_capacity = 0;
static int global_count = 0;

static uint32_t hash_combine(uint32_t a, uint32_t b) {
    return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
}

static GlobalEntry* find_entry(GlobalEntry* entries, int capacity, ObjString* key) {
    uint32_t index = key->hash % capacity;
    GlobalEntry* tombstone = NULL;
    
    while (true) {
        GlobalEntry* entry = &entries[index];
        
        if (!entry->exists) {
            if (entry->key == NULL) {
                return tombstone != NULL ? tombstone : entry;
            } else {
                if (tombstone == NULL) tombstone = entry;
            }
        } else if (entry->key == key) {
            return entry;
        }
        
        index = (index + 1) % capacity;
    }
}

static void adjust_capacity(int capacity) {
    GlobalEntry* entries = calloc(capacity, sizeof(GlobalEntry));
    
    for (int i = 0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].exists = false;
    }
    
    global_count = 0;
    for (int i = 0; i < global_capacity; i++) {
        GlobalEntry* entry = &global_table[i];
        if (!entry->exists) continue;
        
        GlobalEntry* dest = find_entry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        dest->exists = true;
        global_count++;
    }
    
    free(global_table);
    global_table = entries;
    global_capacity = capacity;
}

bool global_get(ObjString* key, Value* value) {
    if (global_count == 0) return false;
    
    GlobalEntry* entry = find_entry(global_table, global_capacity, key);
    if (!entry->exists) return false;
    
    *value = entry->value;
    return true;
}

void global_set(ObjString* key, Value value) {
    if (global_count + 1 > global_capacity * 0.75) {
        int capacity = global_capacity < 8 ? 8 : global_capacity * 2;
        adjust_capacity(capacity);
    }
    
    GlobalEntry* entry = find_entry(global_table, global_capacity, key);
    bool is_new = !entry->exists;
    
    if (is_new && entry->key == NULL) global_count++;
    
    entry->key = key;
    entry->value = value;
    entry->exists = true;
}

bool global_delete(ObjString* key) {
    if (global_count == 0) return false;
    
    GlobalEntry* entry = find_entry(global_table, global_capacity, key);
    if (!entry->exists) return false;
    
    entry->exists = false;
    return true;
}

void free_globals() {
    free(global_table);
    global_table = NULL;
    global_capacity = 0;
    global_count = 0;
}
