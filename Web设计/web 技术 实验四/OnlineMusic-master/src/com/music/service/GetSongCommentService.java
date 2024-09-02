package com.music.service;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import com.music.dao.GetSongCommentDao;
import com.music.domain.SongComment;

public class GetSongCommentService {
	public List<Map<String, Object>> getComment(String sid,String uid,String method) throws SQLException {
		GetSongCommentDao dao = new GetSongCommentDao();
		List<Map<String, Object>> list = dao.getComment(sid,uid,method);
		return list;
	}

}
