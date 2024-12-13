#include "Handlers/index.h"
#include "Utils/memmory.h"
#include "Utils/logger.h"
#include "DataTypes/dictionary.h"

Router r;
RouterConfig RouterCofig = {
    .path = "/", // ADD PATH
    .get = ENABLED,
    .post = ENABLED,
    .put = DISABLED,
    .delete = DISABLED,
};

void getIndex(SOCKET *s, request req)
{
    view_or_404(s, req, "index.html");
}

void postIndex(SOCKET *s, request req)
{
    Dict *dict = createDict();
    addToDict(dict, "Id", "1234", STR);
    addToDict(dict, "Username", "Leonidas", STR);
    addToDict(dict, "Password", "DOULEPSE GAMW THN PANAGIA SOU", STR);
    Dict *subdict = createDict();
    addToDict(subdict, "Id_ins", "test", STR);
    addToDict(dict, "Subdict", subdict, DICT);
    char *json = flushDictToJson(dict);
    logInfo("ResJson: %s", json);
    json_200(s, req, json);

    // Cleanup
    deallocate(json);
    cleanupDictionary(dict);
    cleanupDictionary(subdict);
}

void putIndex(SOCKET *s, request req)
{
}

void deleteIndex(SOCKET *s, request req)
{
}

Router getIndexRoutes()
{
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