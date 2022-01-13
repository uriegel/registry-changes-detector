const { register } = require("./index.node")

function registerAddon(params, cb) {
    params["cb"] = cb
    let handle = register(params)
    params["handle"] = handle
    return params
}

function getDWord(params, defValue) {
    return 123
}

function unregister(params) {
}

exports.register = registerAddon
exports.getDWord = getDWord
exports.unregister = unregister