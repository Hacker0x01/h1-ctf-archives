package com.hackerone.candidatevote;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.squareup.picasso.Picasso;

import java.util.List;

/**
 * Created by breadchris on 2/13/18.
 */

public class MyCandidateAdapter extends ArrayAdapter<Candidate> {

    List<Candidate> candidateList;
    Context context;
    private LayoutInflater mInflater;

    // Constructors
    public MyCandidateAdapter(Context context, List<Candidate> objects) {
        super(context, 0, objects);
        this.context = context;
        this.mInflater = LayoutInflater.from(context);
        candidateList = objects;
    }

    @Override
    public Candidate getItem(int position) {
        return candidateList.get(position);
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        final ViewHolder vh;
        if (convertView == null) {
            View view = mInflater.inflate(R.layout.layout_row_view, parent, false);
            vh = ViewHolder.create((RelativeLayout) view);
            view.setTag(vh);
        } else {
            vh = (ViewHolder) convertView.getTag();
        }

        Candidate item = getItem(position);

        vh.textViewName.setText(item.getName());
        vh.textViewVotes.setText("Votes: " + item.getVotes());
        Picasso.with(context).load(item.getUrl()).placeholder(R.mipmap.ic_launcher).error(R.mipmap.ic_launcher).into(vh.imageView);

        return vh.rootView;
    }

    private static class ViewHolder {
        public final RelativeLayout rootView;
        public final ImageView imageView;
        public final TextView textViewName;
        public final TextView textViewVotes;

        private ViewHolder(RelativeLayout rootView, ImageView imageView, TextView textViewName, TextView textViewVotes) {
            this.rootView = rootView;
            this.imageView = imageView;
            this.textViewName = textViewName;
            this.textViewVotes = textViewVotes;
        }

        public static ViewHolder create(RelativeLayout rootView) {
            ImageView imageView = (ImageView) rootView.findViewById(R.id.imageView);
            TextView textViewName = (TextView) rootView.findViewById(R.id.textViewName);
            TextView textViewVotes = (TextView) rootView.findViewById(R.id.textViewVotes);
            return new ViewHolder(rootView, imageView, textViewName, textViewVotes);
        }
    }
}
