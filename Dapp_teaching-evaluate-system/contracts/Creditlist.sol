pragma solidity >=0.4.22 <0.6.0;

contract Creditlist {
    address public minter;//拥有者地址
    // uint num;//名单数量
    // uint requ_num;//请求人数
    // uint add_num;//已添加人数
    string self_name;//拥有者的id
    
    ev[] evlist;
    person[] persons;//名单
    requ_person[] requ_persons;//请求权限名单
    requ_person[] add_persons;//同意添加权限的名单
    //名单的每一个条目信息
    struct person {
        string name;//姓名
        // uint content;//评价
        uint num;
        uint[] list;
    }
    
    struct ev {
        int content;//评价
        uint power;//权重
        string note;//备注
    }
    
    struct requ_person {
        address id;//请求人的地址
        string name;//请求人的姓名
        uint depart;
        uint job;
        string note;//备注
        uint boo;//是否有效
    }
    //构造函数
    constructor() public{
        minter = msg.sender;
        // num = 0;
        // requ_num = 0;
        // add_num = 1;
        //将创建者的地址加入权限名单
        add_persons.push(requ_person({
            id:msg.sender,
            name: "None",
            depart: 0,
            job: 0,
            note:"None",
            boo:1
        }));
        self_name = "None";
    }
    //事件每次名单条目的添加
    event information(string newname,int amount,uint quanzong);
    //设置本人id
    function setname(string memory newname) public {
        require(msg.sender == minter,
        "No right to see information.");
        self_name = newname;
    }
    //输入一条信息
    function write(int amount, uint quanzhong,string memory newname,string memory newnote) public {
        uint b=0;
        // require(msg.sender == minter,
        // "No right to see information.");
        evlist.push(ev({
            content:amount,
            power: quanzhong,
            note: newnote
        }));
        for(uint p = 0;p<persons.length;p++){
            if(keccak256(abi.encodePacked(persons[p].name)) == keccak256(abi.encodePacked(newname))){
              persons[p].list.push(evlist.length-1);
              persons[p].num += 1;
            //   persons[p].content += amount*quanzhong;
              b=1;
              break;
            }
        }
        if(b == 0){
            uint[] memory newlist;
            newlist = new uint[](1);
            newlist[0] = evlist.length-1;
            persons.push(person({
                name: newname,
                // content: amount*quanzhong,
                num: 1,
                list: newlist
            }));
            //persons[persons.length-1].list.push(evlist.length-1);
        }
        emit information(newname,amount,quanzhong);
    }
    //输出某个信息
    function out(uint amount) public returns (string memory,uint,uint[] memory){
        // uint b=0;
        // //判断是否是权限名单中的用户
        // for(uint p = 0;p<add_persons.length;p++){
        //     if(add_persons[p].id == msg.sender){
        //       b = 1;
        //       break;
        //     }
        // }
        // require(b == 1,
        // "No right to see information.");
        // //请求输出的不在名单范围中
        require(amount <persons.length,
        "The search item does not exist.");
        return (persons[amount].name,persons[amount].num,persons[amount].list);
    }
    //得到名单条目数量
    function getnum() public returns(uint){
        return persons.length;
    }
    //得到同意拥有权限的人数
    function getaddnum() public returns(uint){
        return add_persons.length;
    }
    //得到请求获取权限的人数
    function getrequnum() public returns(uint){
        return requ_persons.length;
    }
    //查看用户是否不可信  
    // function getanswwer(string memory input) public returns(bool,uint,uint){
    //     require(msg.sender == minter,
    //     "No right to see information.");
    //     for(uint p = 0;p<persons.length;p++){
    //         if(keccak256(abi.encodePacked(persons[p].name)) == keccak256(abi.encodePacked(input))){
    //             return (true,p,persons[p].content);
    //         }
    //     }
    //     return (false,0,0);
    // }
    function getevnum() public returns(uint){
        return evlist.length;
    }
    function get_sender() public returns(address){
        return msg.sender;
    }
    function get_minder() public returns(address){
        return minter;
    }
    function showev(uint amount) public returns (int,uint,string memory){
        // uint b=0;
        // //判断是否是权限名单中的用户
        // for(uint p = 0;p<add_persons.length;p++){
        //     if(add_persons[p].id == msg.sender){
        //       b = 1;
        //       break;
        //     }
        // }
        // require(b == 1,
        // "No right to see information.");
        //请求输出的不在名单范围中
        require(amount <evlist.length,
        "The search item does not exist.");
        return (evlist[amount].content,evlist[amount].power,evlist[amount].note);
    }
    //请求添加
    function requminer(string memory newname,uint newdepart,uint newjob,string memory newnote) public returns(uint){
        // require(msg.sender != minter,
        // "You have permission, no need to apply for additions.");
        requ_persons.push(requ_person({
            id: msg.sender,
            name: newname,
            depart: newdepart,
            job: newjob,
            note: newnote,
            boo: 1
        }));
        //requ_num=requ_num+1;
        return requ_persons.length;
    }
    //同意添加权限
    function addminer(uint amount) public returns(uint){
        require(msg.sender == minter,
        "No right to see information.");
        //请求添加的不在请求名单范围中
        require(amount <requ_persons.length,
        "The search item does not exist.");
        //添加进权限名单
        add_persons.push(requ_person({
            id: requ_persons[amount].id,
            name: requ_persons[amount].name,
            depart: requ_persons[amount].depart,
            job: requ_persons[amount].job,
            note: "None",
            boo: 1
        }));
        requ_persons[amount].boo = 0;
        // add_num=add_num+1;
        return add_persons.length;
    }
    function delereminer(uint amount) public returns(bool){
        require(msg.sender == minter,
        "No right to see information.");
        //请求添加的不在请求名单范围中
        require(amount <requ_persons.length,
        "The search item does not exist.");
        requ_persons[amount].boo = 0;
        return true;
    }
    function deleminer(uint amount) public returns(bool){
        require(msg.sender == minter,
        "No right to see information.");
        //请求添加的不在请求名单范围中
        require(amount <add_persons.length,
        "The search item does not exist.");
        add_persons[amount].boo = 0;
        return true;
    }
    //得到相应的已添加权限名单信息
    function getaddperson(uint number) public returns(bool,string memory,address,uint,uint,string memory,uint){
         require(number <add_persons.length,
        "The search item does not exist.");
         return (true,add_persons[number].name,add_persons[number].id,add_persons[number].depart,add_persons[number].job,add_persons[number].note,add_persons[number].boo);
    }
    function getrequperson(uint number) public returns(bool,string memory,address,uint,uint,string memory,uint){
         require(number <requ_persons.length,
        "The search item does not exist.");
         return (true,requ_persons[number].name,requ_persons[number].id,requ_persons[number].depart,requ_persons[number].job,requ_persons[number].note,requ_persons[number].boo);
    }
}