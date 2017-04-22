#include "lua.h"
#include "lauxlib.h"
#include "fs_file.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>


struct lfs_handle {
    struct fs_file* f;
};


#define check_op(p, msg) do{ \
    if((p)->f == NULL) { \
        luaL_error(L, msg); \
    } \
}while(0)


static struct lfs_handle*
_self(lua_State* L) {
    luaL_checktype(L, 1, LUA_TUSERDATA);
    return (struct lfs_handle*)lua_touserdata(L, 1);
}


static int
lfsfile_read(lua_State* L) {
    struct lfs_handle* p = _self(L);
    check_op(p, "read invalid lfsfile_handle");
    int sz = luaL_optinteger(L, 2, 1);
    if(sz <= 0) {
        luaL_error(L, "invalid read sz:%d", sz);
    }
    unsigned char def_buff[64];
    unsigned char* buff = (sz<=sizeof(def_buff))?(def_buff):((unsigned char*)malloc((size_t)sz));
    int ret = fs_read(p->f, buff, sz);
    if(ret>=0) {
        assert(ret <= sz);
        lua_pushlstring(L, (const char*)buff, ret);
    }

    if(buff != def_buff) {
        free(buff);
    }

    if(ret <0) {
        luaL_error(L, "read error: %d", ret);
    }
    return 1;
}


static int
lfsfile_write(lua_State* L) {
    struct lfs_handle* p = _self(L);
    check_op(p, "write invalid lfsfile_handle");
    size_t len = 0;
    const char* s = lua_tolstring(L, 2, &len);
    if(s==NULL || len==0) {
        luaL_error(L, "write invalid string");
    }
    int ret = fs_write(p->f, (void*)s, (size_t)len);
    if(ret<0) {
        luaL_error(L, "write error: %d", ret);
    }
    return 0;
}


static int
lfsfile_seek(lua_State* L) {
    struct lfs_handle* p = _self(L);
    check_op(p, "seek invalid lfsfile_handle");
    const char* fmt = luaL_optstring(L, 2, "cur");
    int offset = luaL_optinteger(L, 3, 0);

    if(strcmp(fmt, "cur")==0) {
        fs_seek_from_cur(p->f, offset);
    }else if(strcmp(fmt, "set")==0) {
        fs_seek_from_head(p->f, offset);
    }

    lua_pushinteger(L, fs_ftell(p->f));
    return 1;
}


static int
lfsfile_size(lua_State* L) {
    struct lfs_handle* p = _self(L);
    check_op(p, "size invalid lfsfile_handle");    
    size_t sz = fs_size(p->f);
    lua_pushinteger(L, sz);
    return 1;
}


static int
lfsfile_close(lua_State* L) {
    struct lfs_handle* p = _self(L);
    if(p->f) {
        fs_close(p->f);
        p->f = NULL;
    }
    return 0;
}


static int
lfsfile_open(lua_State* L) {
    const char* path = luaL_checkstring(L, 1);
    const char* fmt  = luaL_checkstring(L, 2);

    struct fs_file* f = fs_open(path, fmt);
    if(!f) {
        luaL_error(L, "open path: %s format:%s error.", path, fmt);
    }

    struct lfs_handle* p = (struct lfs_handle*)lua_newuserdata(L, sizeof(struct lfs_handle));
    p->f = f;
    if(luaL_newmetatable(L, "fsfile_metatable")) {
        luaL_Reg l[] = {
            {"read", lfsfile_read},
            {"write", lfsfile_write},
            {"size", lfsfile_size},
            {"seek", lfsfile_seek},
            {"close", lfsfile_close},
            { NULL, NULL },
        };
        luaL_newlib(L, l);
        lua_setfield(L, -2, "__index");
        lua_pushcfunction(L, lfsfile_close);
        lua_setfield(L, -2, "__gc");
    }
    lua_setmetatable(L, -2);

    return 1;
}


int
luaopen_fsfile_c(lua_State* L) {
    luaL_Reg reg[] = {
        {"open", lfsfile_open},
        {"close", lfsfile_close},
        { NULL, NULL },
    };

    luaL_checkversion(L);
    luaL_newlib(L, reg);
    return 1;
}