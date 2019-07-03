using Miao_studio.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Windows.Forms;

namespace Miao_studio.Controllers
{
    public class UserController : Controller
    {
        private userAccount db = new userAccount();
        // GET: User
        public ActionResult Login()
        {
            return View();
        }

        [HttpPost]
        public ActionResult Login(string username, string password)
        {
            if ((username == "") || (password == ""))
            {
                MessageBox.Show("用户名或者密码不能为空", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login");
            }
            else
            {
                foreach (var i in db.accounts.ToList())
                {
                    if ((i.username == username) && (i.password == password))
                    {
                        MessageBox.Show("登录成功", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                        Session["user"] = i.name;
                        return RedirectToAction("Index");
                    }
                }
                MessageBox.Show("用户名或者密码错误", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login");
            }
        }

        public ActionResult Index()
        {
            if (Session["user"] == null)
            {
                MessageBox.Show("请先登录", "登录提示", MessageBoxButtons.OK, MessageBoxIcon.Information, MessageBoxDefaultButton.Button1, MessageBoxOptions.DefaultDesktopOnly);
                return RedirectToAction("Login");
            }
            return View();
        }

        public ActionResult Logout()
        {
            Session["user"] = null;
            return RedirectToAction("Login");
        }
    }
}