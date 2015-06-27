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
local rs = prequire( "gamewalker", tb , function() print('oh no!') end, {onSucess=function() print('fake'); end})
print('rs =>', rs)
print('tb size --->', #tb)


local rs = prequire( "webgamewalker", tb , function() print('oh no!') end, {onSucess=function() print('fake'); end})
print('rs =>', rs)
print('tb size --->', #tb)
