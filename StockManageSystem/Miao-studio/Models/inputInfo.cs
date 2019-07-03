namespace Miao_studio.Models
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;

    public partial class inputInfo : DbContext
    {
        public inputInfo()
            : base("name=inputInfo1")
        {
        }

        public virtual DbSet<input> inputs { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
        }
    }
}
