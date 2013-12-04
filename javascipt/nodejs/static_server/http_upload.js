var http = require('http');
var formidable = require('formidable');

function show(res) {
    var html = ''
    + '<form method="post" action="/" enctype="multipart/form-data">'
    + '<p><input type="text" name="name" /></p>'
    + '<p><input type="file" name="file" /></p>'
    + '<p><input type="submit" value="Upload" /></p>'
    + '</form>';

    res.setHeader('Content-Type', 'text/html');
    res.setHeader('Content-Length', Buffer.byteLength(html));
    res.end(html);
}

function notFound(res) {
    res.statusCode = 404;
    res.setHeader('Content-Type', 'text/plain');
    res.end('Not Found');
}

function badRequest(res) {
    res.statusCode = 400;
    res.setHeader('Content-Type', 'text/plain');
    res.end('Bad Request');
}

function isFormData(req) {
    var type = req.headers['Content-type'] || '';
    return 0 == type.indexOf('multipart/form-data');
}

function upload(req, res) {
    if (!isFormData(req)) {
        res.statusCode = 400;
        res.end('Bad request: expecting multipart/form-data');
        return;
    }
    var form = new formidable.IncomingForm();
    form.parse(req);
}

var server = http.createServer(function(req, res) {
    if ('/' == req.url) {
        switch (req.method) {
        case 'GET':
            show(res);
            break;
        case 'POST':
            upload(req, res);
            break;
        default:
            badRequest(res);
            }
    } else {
        notFound(res);
    }
});

server.listen(3000)
