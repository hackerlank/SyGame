require("json")
require("base64")

test = {
  one='first',two='second',three={2,3,5}
}

jsonTest = json.encode(test)
print('JSON encoded test is: ' .. jsonTest)
local PAR_TABLE =  
{  
    MAPNAME = "����",  
    MONSTER = {"��˹��", 1, 1},  --����Ϊ������Ҳ������Ϊ������  
}  
print(PAR_TABLE.MAPNAME)
print(PAR_TABLE.MONSTER[2])

local NET_COMMAND_ID={
	LOGIN_IN = 1,
	RET_LOGIN = 2,
}

print("Array test OK"..a[9])
--a[0].test = "HHHEHEHEHHEHEHEHEHEHEHE"
--print(a[0].test)
Monster={
	onclick=function()
		print("jack i am")
	end,
}

Monster.onclick=function(d) print(" jack i changed"..d) end

Monster.onclick(100)

function testFunctionBack(i)
	print("....testFunctionBack...."..i)
end

local funcStr = base64.funstring(testFunctionBack)

--bind(funcStr,funcStrLen)


local func = base64.stringfun(funcStr)(111)
print("=================================")
obj = TestObject:new_local()
print(obj)
obj:callFunc(base64.funstring(function(i) -- ����ʽ��� ����Ļ����˱�̵�ģʽ ʹ�������е�UI ���ļ��ص� �Լ�����ص�
	print("hello,world"..i)
	print(obj)
end))

obj:doFunc()



