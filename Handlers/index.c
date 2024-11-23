#include "Handlers/index.h"

Router r;
RouterConfig RouterCofig = {
    .path = "/", // ADD PATH
    .get = ENABLED,
    .post = DISABLED,
    .put = DISABLED,
    .delete = DISABLED,
};

void getIndex(SOCKET *s, request req)
{
    view_or_404(s, req, "index.html");
}

void postIndex(SOCKET *s, request req)
{
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