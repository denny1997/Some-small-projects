namespace Miao_studio.Models
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;

    public partial class stockInfo : DbContext
    {
        public stockInfo()
            : base("name=stockInfo")
        {
        }

        public virtual DbSet<stock> stocks { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
        }
    }
}
