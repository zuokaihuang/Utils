msg='sucess'
print(msg)
local tb = {
    onSucessq=function ()
        print("haha onSucess");
        return 1;
    end
    ,
    onError=function()
                print("oh no onError");
            end
}

function load_local()
    local rs = prequire( "gamewalker", tb , function() print('oh no!') end, {onSucess=function() print('fake'); end})
    print('rs =>', rs)
    print('tb size --->', #tb)
end


function load_web()
    local rs = prequire( "webgamewalker", tb , function() print('oh no!') end, {onSucess=function() print('fake'); end})
    print('rs =>', rs)
    print('tb size --->', #tb)
end

load_local()
load_web()
