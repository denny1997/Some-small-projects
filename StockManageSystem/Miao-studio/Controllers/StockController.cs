using Miao_studio.Models;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Windows.Forms;

namespace Miao_studio.Controllers
{
    public class StockController : Controller
    {
        private stockInfo db = new stockInfo();
        private inputInfo db1 = new inputInfo();
        private outputInfo db2 = new outputInfo();
        // GET: Stock
        public ActionResult Index()
        {
            if (Session["user"] == null)
            {
                MessageBox.Show("请先登录", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login","User");
            }
            return View(db.stocks.ToList());
        }

        [HttpPost]
        public ActionResult StockDetail(string id)
        {
            if (Session["user"] == null)
            {
                MessageBox.Show("请先登录", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login", "User");
            }
            stock s = db.stocks.Find(int.Parse(id));
            var records = db1.inputs.AsQueryable();
            records = records.Where(c => ((c.name == s.name) && (c.type == s.type))).OrderByDescending(c => c.Id);
            List<input> stk = new List<input>();
            int ts = s.stock1;
            foreach (var item in records)
            {
                if (item.number >= ts)
                {
                    item.number = ts;
                    stk.Add(item);
                    break;
                }
                else
                {
                    stk.Add(item);
                    ts -= item.number;
                }
            }
            stk.Reverse();
            string details = "名称："+s.name+" | 型号："+s.type+"\n";
            foreach(var item in stk)
            {
                details += "入库日期：" + item.time.ToLongDateString() + " | 库存数：" + item.number.ToString() + " | 单价：" + item.price.ToString() + "\n";
            }
            return Content(details);
        }

        public ActionResult Input()
        {
            if (Session["user"] == null)
            {
                MessageBox.Show("请先登录", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login", "User");
            }
            return View();
        }

        [HttpPost]
        public ActionResult Input([Bind(Include = "Id,name,type,number,unit,price,time,_operator,provider")] input input)
        {
            if (Session["user"] == null)
            {
                MessageBox.Show("请先登录", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login", "User");
            }
            DateTime now = DateTime.Now;
            var records = db1.inputs.AsQueryable();
            records = records.Where(c => ((c.time.Month == now.Month)&&(c.time.Year == now.Year)));
            int num = records.Count()+1;
            input.Id = now.ToString("yyyy-MM") + "-" + num.ToString();
            input._operator = Session["user"].ToString();
            db1.inputs.Add(input);
            db1.SaveChanges();
            bool flag = true;
            foreach(var i in db.stocks.ToList())
            {
                if((i.name == input.name)&&(i.type == input.type))
                {
                    i.stock1 += input.number;
                    i.total += input.number * input.price;
                    db.Entry(i).State = EntityState.Modified;
                    db.SaveChanges();
                    flag = false;
                    break;
                }
            }
            if(flag)
            {
                stock newRecord = new stock();
                newRecord.name = input.name;
                newRecord.type = input.type;
                newRecord.stock1 = input.number;
                newRecord.total = input.number * input.price;
                newRecord.unit = input.unit;
                db.stocks.Add(newRecord);
                db.SaveChanges();
            }
            MessageBox.Show("入库成功", "入库提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
            return RedirectToAction("Index");
        }

        public ActionResult InputDelete(string id)
        {
            if (Session["user"] == null)
            {
                MessageBox.Show("请先登录", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login", "User");
            }
            input record = db1.inputs.Find(id);            
            foreach (var i in db.stocks.ToList())
            {
                if ((i.name == record.name) && (i.type == record.type))
                {
                    i.stock1 -= record.number;
                    i.total -= record.number * record.price;
                    db.Entry(i).State = EntityState.Modified;
                    db.SaveChanges();                    
                    break;
                }
            }
            db1.Entry(record).State = EntityState.Deleted;
            db1.SaveChanges();
            MessageBox.Show("删除成功", "操作提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
            return RedirectToAction("Index");
        }

        [HttpPost]
        public ActionResult Output(string Id,string number,string project)
        {
            if (Session["user"] == null)
            {
                MessageBox.Show("请先登录", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login", "User");
            }
            stock s = db.stocks.Find(int.Parse(Id));
            int num = int.Parse(number);
            if(num>s.stock1)
            {
                return Content("库存不足");
            }
            var records = db1.inputs.AsQueryable();
            records = records.Where(c => ((c.name == s.name) && (c.type == s.type))).OrderByDescending(c => c.Id);
            List<input> stk = new List<input>();
            int ts = s.stock1;
            foreach( var item in records)
            {
                if (item.number >= ts)
                {
                    item.number = ts;
                    stk.Add(item);
                    break;
                }
                else
                {
                    stk.Add(item);
                    ts -= item.number;
                }                   
            }
            stk.Reverse();
            out_put op = new out_put();
            string details = "";
            ts = num;
            decimal total = 0;
            foreach(var item in stk)
            {
                if(item.number >= ts)
                {
                    details += "数量：" + ts.ToString() + " | 单价：￥" + item.price.ToString();
                    total += ts * item.price;
                    break;
                }
                else
                {
                    details += "数量：" + item.number.ToString() + " | 单价：￥" + item.price.ToString() + "\n";
                    total += item.number * item.price;
                    ts -= item.number;
                }
            }
            DateTime now = DateTime.Now;
            var _records = db2.out_put.AsQueryable();
            _records = _records.Where(c => ((c.time.Month == now.Month) && (c.time.Year == now.Year)));
            int _num = _records.Count() + 1;
            op.Id = now.ToString("yyyy-MM") + "-" + _num.ToString();
            op.name = s.name;
            op.type = s.type;
            op.detail = details;
            op.time = now.Date;
            op._operator = Session["user"].ToString();
            op.project = project;
            op.total = total;
            op.number = num;
            op.unit = s.unit;
            db2.out_put.Add(op);
            db2.SaveChanges();
            s.stock1 -= num;
            s.total -= total;
            db.Entry(s).State = EntityState.Modified;
            db.SaveChanges();
            return Content("出库成功");
        }

        public ActionResult OutputResult(string id, string name, string type, DateTime? begin, DateTime? end, string project)
        {
            if (Session["user"] == null)
            {
                MessageBox.Show("请先登录", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login", "User");
            }
            SearchInfo info = new SearchInfo(id, name, type, begin, end, project);
            var op = db2.out_put.AsQueryable();
            if(!String.IsNullOrEmpty(id))
            {
                op = op.Where(c => c.Id.Contains(id));
            }
            if(!String.IsNullOrEmpty(name))
            {
                op = op.Where(c => c.name.Contains(name));
            }
            if(!String.IsNullOrEmpty(type))
            {
                op = op.Where(c => c.type.Contains(type));
            }
            if (!String.IsNullOrEmpty(project))
            {
                op = op.Where(c => c.project.Contains(project));
            }
            if (begin!=null)
            {
                op = op.Where(c => c.time >= begin);
            }
            if(end!=null)
            {
                op = op.Where(c => c.time <= end);
            }
            return View(Tuple.Create(op.AsEnumerable(),info));
        }

        public ActionResult InputResult(string id, string name, string type, DateTime? begin, DateTime? end,string provider)
        {
            if (Session["user"] == null)
            {
                MessageBox.Show("请先登录", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login", "User");
            }
            SearchInfo info = new SearchInfo(id, name, type, begin, end, provider);
            var ip = db1.inputs.AsQueryable();
            if (!String.IsNullOrEmpty(id))
            {
                ip = ip.Where(c => c.Id.Contains(id));
            }
            if (!String.IsNullOrEmpty(name))
            {
                ip = ip.Where(c => c.name.Contains(name));
            }
            if (!String.IsNullOrEmpty(type))
            {
                ip = ip.Where(c => c.type.Contains(type));
            }
            if (!String.IsNullOrEmpty(provider))
            {
                ip = ip.Where(c => c.provider.Contains(provider));
            }
            if (begin != null)
            {
                ip = ip.Where(c => c.time >= begin);
            }
            if (end != null)
            {
                ip = ip.Where(c => c.time <= end);
            }
            return View(Tuple.Create(ip.AsEnumerable(), info));
        }

        public ActionResult StockResult(string name, string type, DateTime? begin, DateTime? end)
        {
            if (Session["user"] == null)
            {
                MessageBox.Show("请先登录", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login", "User");
            }
            
            SearchInfo info = new SearchInfo(name, type, begin, end);
            var stk = db.stocks.AsQueryable().AsEnumerable();           
            var ip = db1.inputs.AsQueryable().AsEnumerable();
            var op = db2.out_put.AsQueryable().AsEnumerable();
            if (!String.IsNullOrEmpty(name))
            {
                stk = stk.Where(c => c.name.Contains(name));
                ip = ip.Where(c => c.name.Contains(name));
                op = op.Where(c => c.name.Contains(name));
            }
            if (!String.IsNullOrEmpty(type))
            {
                stk = stk.Where(c => c.type.Contains(type));
                ip = ip.Where(c => c.name.Contains(type));
                op = op.Where(c => c.name.Contains(type));
            }

            if (begin != null)
            {
                ip = ip.Where(c => c.time >= begin);
                op = op.Where(c => c.time >= begin);
            }
            //List<stock> t1 = stk.ToList();
            List<StockResult> former = RetroSpect(stk, ip, op);
           
            List<StockResult> latter;
            if (end != null)
            {
                end = end.Value.AddMonths(1);
                ip = ip.Where(c => c.time >= end);
                op = op.Where(c => c.time >= end);
                latter = RetroSpect(stk, ip, op);
            }
            else
            {
                latter = RetroSpect(stk, null, null);
            }
            
           
            return View(Tuple.Create(former, latter, info));
        }

        public FileResult ExportStockExcel(DateTime begin, DateTime end)
        {
            NPOI.HSSF.UserModel.HSSFWorkbook stockExcel = new NPOI.HSSF.UserModel.HSSFWorkbook();
            NPOI.SS.UserModel.ISheet sheet1 = stockExcel.CreateSheet("Sheet1");
            NPOI.SS.UserModel.IRow row1 = sheet1.CreateRow(0);
            row1.CreateCell(0).SetCellValue("名称");
            row1.CreateCell(1).SetCellValue("型号");
            row1.CreateCell(2).SetCellValue("单位");
            row1.CreateCell(3).SetCellValue("期初库存");
            row1.CreateCell(4).SetCellValue("期初总金额");
            row1.CreateCell(5).SetCellValue("期间入库数量");
            row1.CreateCell(6).SetCellValue("期间入库金额");
            row1.CreateCell(7).SetCellValue("期间出库数量");
            row1.CreateCell(8).SetCellValue("期间出库金额");
            row1.CreateCell(9).SetCellValue("期末库存");
            row1.CreateCell(10).SetCellValue("期末总金额");

            List<StockExcel> res = (List<StockExcel>)Session["excel"];
            for (int i = 0; i < res.Count; i++)
            {
                NPOI.SS.UserModel.IRow rowtemp = sheet1.CreateRow(i + 1);
                rowtemp.CreateCell(0).SetCellValue(res[i].name.ToString());
                rowtemp.CreateCell(1).SetCellValue(res[i].type.ToString());
                rowtemp.CreateCell(2).SetCellValue(res[i].unit.ToString());
                rowtemp.CreateCell(3).SetCellValue(res[i].former_stock.ToString());
                rowtemp.CreateCell(4).SetCellValue(res[i].former_total.ToString());
                rowtemp.CreateCell(5).SetCellValue(res[i].delta_inputstock.ToString());
                rowtemp.CreateCell(6).SetCellValue(res[i].delta_inputtotal.ToString());
                rowtemp.CreateCell(7).SetCellValue(res[i].delta_outputstock.ToString());
                rowtemp.CreateCell(8).SetCellValue(res[i].delta_outputtotal.ToString());
                rowtemp.CreateCell(9).SetCellValue(res[i].latter_stock.ToString());
                rowtemp.CreateCell(10).SetCellValue(res[i].latter_total.ToString());
            }

            MemoryStream ms = new MemoryStream();
            stockExcel.Write(ms);
            ms.Seek(0, SeekOrigin.Begin);

            string fileName = begin.ToString("yyyy-MM") + "到" + end.ToString("yyyy-MM") + "库存汇总表.xls";
            return File(ms, "application/vnd.ms-excel", fileName);
        }

        private List<StockResult> RetroSpect(IEnumerable<stock> stock, IEnumerable<input> input, IEnumerable<out_put> output)
        {
            List<StockResult> res = new List<StockResult>();
            foreach (var item in stock)
            {
                int temp_stk = item.stock1;
                decimal temp_total = item.total;
                int temp_dstock = 0;
                decimal temp_dtotal = 0;
                if ((input!=null)&&(output!=null))
                {
                    var temp_ip = input.Where(c => ((c.name == item.name) && (c.type == item.type)));
                    var temp_op = output.Where(c => ((c.name == item.name) && (c.type == item.type)));                    
                    foreach (var it in temp_ip)
                    {
                        temp_stk -= it.number;
                        temp_total -= it.number * it.price;
                        temp_dstock += it.number;
                    }
                    foreach (var it in temp_op)
                    {
                        temp_stk += it.number;
                        temp_total += it.total;
                        temp_dtotal += it.total;
                    }
                }               
                res.Add(new StockResult(item.name,item.type,item.unit,temp_stk,temp_total,temp_dstock,temp_dtotal));
            }
            return res;
        }
    }
}