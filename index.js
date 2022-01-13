const { register } = require("./index.node")

function registerAddon(params, cb) {
    let handle = register(params.key, params.value)
    params["handle"] = handle

// setInterval()

    return params
}

function getDWord(params, defValue) {
    return 123
}

function unregister(params) {
    // clearInterval
}

exports.register = registerAddon
exports.getDWord = getDWord
exports.unregister = unregister