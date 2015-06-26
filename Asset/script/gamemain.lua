msg='sucess'
print(msg)
local tb = {
    onSucess=function ()
        print("haha onSucess");
        return 1;
    end
}
local rs = prequire( "hello world", tb , function() print('oh no!') end, {onSucess=function() print('fake'); end})
print('rs =>', rs)
print('tb size --->', #tb)


