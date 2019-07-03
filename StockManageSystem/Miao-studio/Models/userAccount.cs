namespace Miao_studio.Models
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;

    public partial class userAccount : DbContext
    {
        public userAccount()
            : base("name=userAccount")
        {
        }

        public virtual DbSet<account> accounts { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
        }
    }
}
