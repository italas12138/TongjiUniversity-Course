package com.music.dao;

import java.sql.SQLException;

import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.handlers.BeanHandler;
import org.apache.commons.dbutils.handlers.ScalarHandler;

import com.music.utils.DButils;

public class UpdateUserinfoDao {

	public String updateUserinfo(String uid, String attr, String newValue) throws NumberFormatException, SQLException {
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		String flag = null;
		System.out.println(newValue);
		if (attr.equals("password")) {
			String oldPsd = newValue.split("&")[0];
			String newPsd = newValue.split("&")[1];
			System.out.println(newValue);
			System.out.println("oldPsd"+oldPsd);
			System.out.println("newPsd"+newPsd);
			String sql = "select password from user where uid=?";
			Object query = qr.query(sql, new ScalarHandler(), Integer.parseInt(uid));
			System.out.println(query);
			System.out.println(query.equals(oldPsd));
			if (query.equals(oldPsd)) { // 旧密码正确  则修改密码
				String sql1 = "update user set password = ? where uid=?";
				int rows = qr.update(sql1, newPsd,Integer.parseInt(uid));
				flag = rows>0? "true" : "false";
			} else { // 就密码错误
				flag = "你的旧密码输入错误，请重新输入";
			}
		} else {
			String sql = "update user set "+attr+"=? where uid=?";
			int rows = qr.update(sql, newValue,Integer.parseInt(uid));
			flag = rows>0? "true" : "false";
		}
		return flag;
	}

}
