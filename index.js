function register(params, cb) {
    params["cb"] = cb
    return params
}

function getDWord(params, defValue) {
    return 123
}

function unregister(params) {

}

exports.register = register
exports.getDWord = getDWord
exports.unregister = unregister