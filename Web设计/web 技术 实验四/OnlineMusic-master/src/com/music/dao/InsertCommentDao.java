package com.music.dao;

import java.sql.SQLException;

import org.apache.commons.dbutils.QueryRunner;

import com.music.domain.SongComment;
import com.music.utils.DButils;

public class InsertCommentDao {

	public boolean InsertComment(SongComment comment) throws SQLException {
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		String sql = "insert into songComment values(?,?,?,?,?,?,?,null)";
		int rows = qr.update(sql, comment.getSid(),comment.getUsername(),comment.getComtent(),comment.getPraiseNum(),comment.getCommentDate(),comment.getPortrait(),comment.getUid());
		return rows>0? true : false;
	}

}
