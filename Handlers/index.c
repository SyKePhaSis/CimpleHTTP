#include "Handlers/index.h"
#include "Utils/memmory.h"
#include "Utils/logger.h"
#include "Utils/fperiodic.h"
#include "DataTypes/dictionary.h"

RouterConfig RouterCofig = {
    .path = "/", // ADD PATH
    .get = ENABLED,
    .post = ENABLED,
    .put = DISABLED,
    .delete = DISABLED,
};

void getIndex(UNUSED(SOCKET *s), UNUSED(request req))
{
    view_or_404(s, req, "index.html");
}

void postIndex(UNUSED(SOCKET *s), UNUSED(request req))
{
    Dict *dict = createDict();
    addToDict(dict, "Id", "123456lol", STR);
    addToDict(dict, "Username", "Konstantinos", STR);
    addToDict(dict, "Password", "DOULEPSE GAMW THN PANAGIA SOU", STR);
    Dict *subdict = createDict();
    addToDict(subdict, "Id_ins", "test", STR);
    addToDict(dict, "Subdict", subdict, DICT);
    char *json = flushDictToJson(dict);
    logInfo("ResJson: %s", json);
    json_200(s, req, json);

    pcleanup(3, TYPE_PAIR(json, STR), TYPE_PAIR(dict, DICT), TYPE_PAIR(subdict, DICT));
}

void putIndex(UNUSED(SOCKET *s), UNUSED(request req))
{
}

void deleteIndex(UNUSED(SOCKET *s), UNUSED(request req))
{
}

Router getIndexRoutes()
{
    Router r;
    r.path = RouterCofig.path;
    if (RouterCofig.get)
        r.get = getIndex;
    if (RouterCofig.post)
        r.post = postIndex;
    if (RouterCofig.put)
        r.put = putIndex;
    if (RouterCofig.delete)
        r.delete = deleteIndex;
    return r;
}