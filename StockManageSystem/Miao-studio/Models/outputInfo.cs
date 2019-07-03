namespace Miao_studio.Models
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;

    public partial class outputInfo : DbContext
    {
        public outputInfo()
            : base("name=outputInfo")
        {
        }

        public virtual DbSet<out_put> out_put { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
        }
    }
}
