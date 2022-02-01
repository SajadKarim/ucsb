#pragma once

#include "ucsb/core/types.hpp"
#include "ucsb/core/db.hpp"

#include "ucsb/unumdb/unumdb.hpp"
#include "ucsb/rocksdb/rocksdb.hpp"
#include "ucsb/leveldb/leveldb.hpp"
#include "ucsb/wiredtiger/wiredtiger.hpp"
#include "ucsb/lmdb/lmdb.hpp"
#include "ucsb/mongodb/mongodb.hpp"

namespace ucsb {

enum class db_kind_t {
    unknown_k,

    unumdb_k,
    rocksdb_k,
    leveldb_k,
    wiredtiger_k,
    lmdb_k,
    mongodb_k,
};

struct factory_t {
    inline std::shared_ptr<db_t> create(db_kind_t kind, bool transactional);
};

inline std::shared_ptr<db_t> factory_t::create(db_kind_t kind, bool transactional) {
    if (transactional) {
        switch (kind) {
        case db_kind_t::unumdb_k: {
            init_file_io_by_libc("./"); // Note: Temporary solution
            return std::make_shared<unum::unumdb_t>();
        }
        case db_kind_t::rocksdb_k:
            return std::make_shared<facebook::rocksdb_gt<facebook::db_mode_t::transactional_k>>();
        default: break;
        }
    }
    else {
        switch (kind) {
        case db_kind_t::unumdb_k: {
            init_file_io_by_libc("./"); // Note: Temporary solution
            return std::make_shared<unum::unumdb_t>();
        }
        case db_kind_t::rocksdb_k: return std::make_shared<facebook::rocksdb_gt<facebook::db_mode_t::regular_k>>();
        case db_kind_t::leveldb_k: return std::make_shared<google::leveldb_t>();
        case db_kind_t::wiredtiger_k: return std::make_shared<mongodb::wiredtiger_t>();
        case db_kind_t::lmdb_k: return std::make_shared<symas::lmdb_t>();
        case db_kind_t::mongodb_k: return std::make_shared<mongo::mongodb_t>();
        default: break;
        }
    }
    return {};
}

inline db_kind_t parse_db(std::string const& name) {
    db_kind_t dist = db_kind_t::unknown_k;
    if (name == "unumdb")
        return db_kind_t::unumdb_k;
    if (name == "rocksdb")
        return db_kind_t::rocksdb_k;
    if (name == "leveldb")
        return db_kind_t::leveldb_k;
    if (name == "wiredtiger")
        return db_kind_t::wiredtiger_k;
    if (name == "lmdb")
        return db_kind_t::lmdb_k;
    if (name == "mongodb")
        return db_kind_t::mongodb_k;
    return dist;
}

} // namespace ucsb