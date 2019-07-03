var count1 = 0;
var minder;
var begin = 0;
var list1 = new Array();
var list2 = new Array();
var itemlist = new Array();
var item_ev = new Array();
var job_list = new Array("大一","大二","大三","大四","研一","研二","博士","博后","其他");
var department_list = new Array("人文学院","信息学院","工学院","理学院","医学院","传媒学院","外语学院");
App = {

  web3Provider: null,

  contracts: {}, 

 init: function() {

  return App.initWeb3(); 

 },

/*加载web3*/

  initWeb3: function() {

    if (typeof web3 !== 'undefined') {

         App.web3Provider = web3.currentProvider

         web3 = new Web3(App.web3Provider);

     } else {

         App.web3Provider = new Web3.providers.HttpProvider("http://localhost:9545") 

        web3 = new Web3(App.web3Provider);

     }

     return App.initContract();

  },

/*初始化合约，获取合约，不需要使用at()的方式；*/

  initContract: function() {

    $.getJSON('Creditlist.json', function(data){

      App.contracts.Creditlist = TruffleContract(data); 

     App.contracts.Creditlist.setProvider(App.web3Provider);

      App.getInfo();

    });



  },  

getInfo: function() {

    var count1 = 0;
    App.contracts.Creditlist.deployed().then(function(instance) {

        return instance.getevnum.call(); 

     }).then(function(result) {

        for(var i=0; i<result; i ++) {
          
          (function(arg){
            App.contracts.Creditlist.deployed().then(function(instance) {
              
              return instance.showev.call(arg); 

           }).then(function(result) {

              item_ev.push({xinyong:Number(result[0]), quanzhong:Number(result[1]),note:result[2]});

            })
          })(i); 
        }


    });  

    App.contracts.Creditlist.deployed().then(function(instance) {

        return instance.getnum.call(); 

     }).then(function(result) {

        for(var i=0; i<result; i ++) {
          
          (function(arg){
            App.contracts.Creditlist.deployed().then(function(instance) {
                
              return instance.out.call(arg); 

            }).then(function(result) {
                
                itemlist.push({name:result[0],xinyong:0, num:Number(result[1]),list:result[2]});
            })
          })(i); 
        }
    }); 
    
    App.contracts.Creditlist.deployed().then(function(instance) {

        return instance.getrequnum.call(); 

     }).then(function(result) {

        for(var i=0; i<result; i ++) {
          
          (function(arg){
            App.contracts.Creditlist.deployed().then(function(instance) {

              return instance.getrequperson.call(arg); 

           }).then(function(result) {

              if(Number(result[6]))
              list2.push({a:result[1],b:Number(result[3]),c:Number(result[4]),d:result[5],e:Number(arg),id:result[2]});

          })
        })(i); 
      }
    }); 

    App.contracts.Creditlist.deployed().then(function(instance) {

        return instance.getaddnum.call(); 

     }).then(function(result) {

        for(var i=0; i<result; i ++) {
          
          (function(arg){
            App.contracts.Creditlist.deployed().then(function(instance) {

                return instance.getaddperson.call(arg); 

            }).then(function(result) {

              if(Number(result[6]))
                list1.push({a:result[1],b:Number(result[3]),c:Number(result[4]),d:Number(arg),id:result[2]});

            })
        })(i); 
      }
    });  

     App.contracts.Creditlist.deployed().then(function(instance) {

          return instance.get_minder.call(); 

       }).then(function(result) {

          minder = result;

      }); 

    
  },

    
  logout: function(){
    var box = document.getElementById("box"),
        box2 = document.getElementById("box2"),
        box3 = document.getElementById("box3"),
        po = document.getElementById("ueser2"),
        repower = document.getElementById("repower"),
        name = document.getElementById("ueser1");
    if(box3.style.display == "block")
      App.show("box3","butt3");
    if(box2.style.display == "block")
      App.show("box2","butt2");
    if(box.style.display == "block")
      App.show("box","butt1");
    name.innerHTML = "未登录";
    po.innerHTML = "请先登录";
    repower.style.display = "none";
  },

  login: function(){
    var name = document.getElementById("ueser1"),
        po = document.getElementById("ueser2"),
        repower = document.getElementById("repower"),
        tem = 0;
    App.contracts.Creditlist.deployed().then(function(instance) {

        return instance.get_sender.call(); 

     }).then(function(result) {

        name.innerHTML = result;

        for (var i=0; i<list1.length; i ++){

          if(list1[i].id == name.innerHTML){
            tem = 1;
            break;
          }
        }
        if(tem == 1){
          po.innerHTML = "已有权限";
        }else{
          po.innerHTML = "没有权限";
          repower.style.display = "block";
        }

    }); 

  },

  add:function (){
    var name = document.getElementById("name"),
        age = document.getElementById("age"),
        power = document.getElementById("power"),
        mark = document.getElementById("mark");
    App.contracts.Creditlist.deployed().then(function(instance) {

        return instance.write(age.value, power.value, name.value, mark.value, {gas: 500000}); 

     }).then(function(result) {

        item_ev.push({xinyong:age.value,quanzhong:power.value,note:mark.value});
        App.find(name.value,age.value*power.value,item_ev.length-1);
        App.load2();
        App.hide_add('adddiv');

    }); 
      
  },

  load2:function (){
      var todo = document.getElementById("todolist"),
          bili1 = document.getElementById("bili1"),
          bili2 = document.getElementById("bili2"),
          bili3 = document.getElementById("bili3"),
          todoString = "",
          xy1=0,
          xy2=0,
          xy3=0,
          total=itemlist.length;

      for (var i=0; i<itemlist.length; i ++){
          todoString += "<tr>"
                  + "<td ></td>" 
                  + "<td >" + itemlist[i].name + "</td>" 
                  + "<td >" + itemlist[i].xinyong + "</td>" 
                  + "<td >" + itemlist[i].num + "</td>" 
                  +"<td>"
                  +"<a href='#' class='btn btn-info'  onclick='App.show_evlist("+i+")'><i class='fa fa-info'></i></a>"
                  +"</td>"
                  +"</tr>"; 
          if(itemlist[i].xinyong>0){
            xy1++;
          }else if(itemlist[i].xinyong==0){
            xy2++;
          }else {
            xy3++;
          }
      }
      todo.innerHTML = todoString;
      xy3 = xy3/total*100;
      xy2 = xy2/total*100;
      xy1 = xy1/total*100;
      bili3.innerHTML = xy3.toFixed(2)+"%";
      bili3.style.width = bili3.innerHTML;
      bili2.innerHTML = xy2.toFixed(2)+"%";
      bili2.style.width = bili2.innerHTML;
      bili1.innerHTML = xy1.toFixed(2)+"%";
      bili1.style.width = bili1.innerHTML;
  },

  show_evlist: function (j){
    var todo = document.getElementById("evlist"),
        text = document.getElementById("evname"),
        name = document.getElementById("ueser1"),
        temp,tem=0,
        todoString = "";
    for (var i=0; i<list1.length; i ++){
      if(list1[i].id == name.innerHTML){
        tem = 1;
        break;
      }
    }
    if(tem == 1){
      text.innerHTML = "用户:" + itemlist[j].name;
      for (var i=0; i<itemlist[j].list.length; i ++){
        temp = itemlist[j].list[i];
        todoString += "<tr>"
                + "<td >" + Number(i) + "</td>" 
                + "<td >" + item_ev[temp].xinyong + "</td>" 
                + "<td >" + item_ev[temp].quanzhong + "</td>" 
                + "<td >" + item_ev[temp].note + "</td>" 
                  +"</tr>"; 
      }
      todo.innerHTML = todoString;
      App.show_add("ev");
    }else{
      alert("请先点击登录按钮");
    }
  },

  find: function (a,b,c){
      for (var i=0; i<itemlist.length; i ++){
          if(itemlist[i].name == a){
              itemlist[i].list.push(c);
              itemlist[i].xinyong = Number(itemlist[i].xinyong)+Number(b);
              itemlist[i].num = itemlist[i].num+1;
              return i;
          }
      }
      var empty = new Array();
      empty.push(c);
      itemlist.push({name:a,xinyong:b,num:1,list:empty});
  },

  request:function (){
      var name = document.getElementById("newname"),
          department = document.getElementById("department"),
          job = document.getElementById("job"),
          note = document.getElementById("note"),
          user = document.getElementById("ueser1"),
          mig = document.getElementById("mig");

      if(name.value!="" && department.value!="--" && job.value!="--" && note.value!=""){
          App.contracts.Creditlist.deployed().then(function(instance) {

              return instance.getrequnum.call(); 

           }).then(function(result) {

              count1 = result;
              list2.push({a:name.value,b:department.value,c:job.value,d:note.value,e:Number(count1),id:user.innerHTML});
              App.load();
              App.hide_add('requdiv');

              App.contracts.Creditlist.deployed().then(function(instance) {

                return instance.requminer(name.value, department.value, job.value, note.value, {gas: 400000}); 

             })
          });
          
      }else {
          alert("不能为空");
      }
  },

  update:function (i,j){
      var todo = document.getElementById("rightlist"),
          name = document.getElementById("ueser1"),
          todoString = todo.innerHTML;
      App.contracts.Creditlist.deployed().then(function(instance) {

          return instance.get_sender.call(); 

       }).then(function(result) {

          if(minder == result){
            App.contracts.Creditlist.deployed().then(function(instance) {

                return instance.getaddnum.call(); 

             }).then(function(result) {

                list1.push({a:list2[i].a,b:Number(list2[i].b),c:Number(list2[i].c),d:result,id:list2[i].id});
                App.remove2(i,j);
                todo.innerHTML = todoString;
                App.load();
                App.contracts.Creditlist.deployed().then(function(instance) {

                return instance.addminer(j, {gas: 500000}); 
              })
            });
          }else{
            alert("只有BOOS才有此权限");
          }

      }); 
      
  },

  load:function (){

      var right = document.getElementById("rightlist"),
          requ = document.getElementById("requlist"),
          todoString1 = "",
          todoString2 = "";
      for (var i=0; i<list1.length; i ++){
        if(list1[i].a =="None"){
          todoString1 += "<tr>"
                + "<td id='p-"+i+"'></td>" 
                + "<td id='p-"+i+"'>" + "BOOS" + "</td>" 
                + "<td id='p-"+i+"'>" + "BOOS"  + "</td>" 
                + "<td id='p-"+i+"'>" + "BOOS" + "</td>" 
                +"<td></td>"
                +"</tr>";  
        }else{
          todoString1 += "<tr>"
                + "<td id='p-"+i+"'></td>" 
                + "<td id='p-"+i+"'>" + list1[i].a + "</td>" 
                + "<td id='p-"+i+"'>" + job_list[list1[i].b]  + "</td>" 
                + "<td id='p-"+i+"'>" + department_list[list1[i].c] + "</td>" 
                +"<td>"
                +"<a href='#' class='btn btn-danger' onclick='App.remove1("+i+","+list1[i].d+")'><i class='fa fa-times'></i></a>"
                +"</td>"
                +"</tr>";  
        }
      }

      for (var i=0; i<list2.length; i ++){
        var department=list2[i].b,
        job = list2[i].c;
        todoString2 += "<tr>"
                + "<td id='p-"+i+"'></td>" 
                + "<td id='p-"+i+"'>" + list2[i].a + "</td>" 
                + "<td id='p-"+i+"'>" + job_list[list2[i].b]  + "</td>" 
                + "<td id='p-"+i+"'>" + department_list[list2[i].c] + "</td>" 
                + "<td id='p-"+i+"'>" + list2[i].d + "</td>" 
                +"<td>"
                +"<a href='#' class='btn btn-success' onclick='App.update("+i+","+list2[i].e+")'><i class='fa fa-check'></i></a>"
                +"<a href='#' class='btn btn-danger' onclick='App.remove2("+i+","+list2[i].e+")'><i class='fa fa-times'></i></a>"
                +"</td>"
                +"</tr>";  
      }
      right.innerHTML = todoString1;
      requ.innerHTML = todoString2;
  },

  remove1: function (i,j){
    var name = document.getElementById("ueser1");
    App.contracts.Creditlist.deployed().then(function(instance) {

        return instance.get_sender.call(); 

     }).then(function(result) {

        if(minder == result){
          App.contracts.Creditlist.deployed().then(function(instance) {

            return instance.deleminer(j, {gas: 500000}); 

          }).then(function(result) {

              list1.splice(i,1);
              App.load();

          }); 
        }else{
          alert("只有BOOS才有删除权限");
        }

    }); 
    
      
  },

  remove2: function (i,j){
    var name = document.getElementById("ueser1");
    App.contracts.Creditlist.deployed().then(function(instance) {

        return instance.get_sender.call(); 

     }).then(function(result) {

        if(minder == result){
          App.contracts.Creditlist.deployed().then(function(instance) {

              return instance.delereminer(j, {gas: 500000}); 

           }).then(function(result) {

                list2.splice(i,1);
                App.load();

          });
        }else{
          alert("只有BOOS才有删除权限");
        }

    }); 
    
      
  },

  show: function (obj,tex){
    var name = document.getElementById("ueser1"),
        box = typeof obj == "string" ? document.getElementById(obj) : obj,
        text= typeof tex == "string" ? document.getElementById(tex) : tex,
        tem=0;
    if(box.style.display=="block"){
      box.style.display = "none";
    }else{
      for (var i=0; i<list1.length; i ++){
        if(list1[i].id == name.innerHTML){
          tem = 1;
          break;
        }
      }
      if(tem == 1){
          for (var j = 0; j <itemlist.length; j++) {
            count1 = 0;
            for(var i=0; i<itemlist[j].list.length; i ++){
              var tem = itemlist[j].list[i];
              count1 += item_ev[tem].xinyong*item_ev[tem].quanzhong; 
            }
            itemlist[j].xinyong=count1;
          }
          box.style.display = "block";
          App.load2();
          App.load(); 
            

      }else{
        alert("请先点击登录按钮 或 申请权限");
      }
    }
       
  },

  show_add:function (obj){ 
      var popUp = typeof obj == "string" ? document.getElementById(obj) : obj;
      popUp.style.visibility = "visible"; 
  },

  hide_add:function (obj){ 
      var popUp = typeof obj == "string" ? document.getElementById(obj) : obj;
      popUp.style.visibility = "hidden"; 
  },


}

 

$(function(){  

$(window).load(function() {

      App.init();

  });

});
