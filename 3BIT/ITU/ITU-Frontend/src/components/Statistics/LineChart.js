import React, {useEffect, useRef, useState} from 'react';
import './LineChart.css';
import * as d3 from 'd3';

const LineChart = ({ data, width = 600, height = 400 }) => {
    const svgRef = useRef();
    const tooltipRef = useRef();

    useEffect(() => {
        const margin = { top: 20, right: 30, bottom: 30, left: 40 };
        const innerWidth = width - margin.left - margin.right;
        const innerHeight = height - margin.top - margin.bottom;

        const xScale = d3.scaleTime()
            .domain(d3.extent(data, d => d.date))
            .range([0, innerWidth]);

        const yScale = d3.scaleLinear()
            .domain([0, d3.max(data, d => d.value)])
            .range([innerHeight, 0]);

        const line = d3.line()
            .x(d => xScale(d.date))
            .y(d => yScale(d.value));

        const svg = d3.select(svgRef.current)
            .attr('width', width)
            .attr('height', height);
        svg.selectAll('*').remove();

        const g = svg.append('g')
            .attr('transform', `translate(${margin.left},${margin.top})`);

        g.append('g').call(d3.axisLeft(yScale));
        g.append('g')
            .attr('transform', `translate(0,${innerHeight})`)
            .call(d3.axisBottom(xScale).ticks(6));

        g.append('path')
            .datum(data)
            .attr('fill', 'none')
            .attr('stroke', 'steelblue')
            .attr('stroke-width', 2)
            .attr('d', line);

        const tooltip = d3.select(tooltipRef.current)
            .style('position', 'absolute')
            .style('opacity', 0)
            .style('background', '#fff')
            .style('padding', '5px')
            .style('border', '1px solid #ccc')
            .style('border-radius', '4px');

        g.selectAll('.dot')
            .data(data)
            .enter()
            .append('circle')
            .attr('class', 'dot')
            .attr('cx', d => xScale(d.date))
            .attr('cy', d => yScale(d.value))
            .attr('r', 5)
            .attr('fill', 'steelblue')
            .on('mouseover', (event, d) => {
                tooltip.style('opacity', 1)
                    .html(`Date: ${d3.timeFormat('%Y-%m-%d')(d.date)}<br>Value: ${d.value}<br>Click to interract`)
                    .style('left', `${event.pageX + 5}px`)
                    .style('top', `${event.pageY - 28}px`);
            })
            .on('mouseout', () => {
                tooltip.style('opacity', 0);
            });

    }, [data, width, height]);

    return (
        <>
            <svg ref={svgRef}></svg>
            <div ref={tooltipRef} className="tooltip"></div>
        </>
    );
};

export default LineChart;