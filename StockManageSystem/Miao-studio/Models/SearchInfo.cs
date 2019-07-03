using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace Miao_studio.Models
{
    public class SearchInfo
    {
        public string id { get; set; }
        public string name { get; set; }
        public string type { get; set; }
        public DateTime? begin { get; set; }
        public DateTime? end { get; set; }
        public string project { get; set; }

        public SearchInfo(string id,string name,string type,DateTime? begin,DateTime? end,string project)
        {
            this.id = id;
            this.name = name;
            this.type = type;
            this.begin = begin;
            this.end = end;
            this.project = project;
        }

        public SearchInfo(string id, string name, string type, DateTime? begin, DateTime? end)
        {
            this.id = id;
            this.name = name;
            this.type = type;
            this.begin = begin;
            this.end = end;
        }

        public SearchInfo(string name, string type, DateTime? begin, DateTime? end)
        {           
            this.name = name;
            this.type = type;
            this.begin = begin;
            this.end = end;
        }
    }
}